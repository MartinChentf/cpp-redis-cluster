#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "redis_client.h"
#include "redis_log.h"
#include "redis_helper.h"
#include "socket_client.h"

static const char* const INFO_CLUSTER =
"*2\r\n"
"$4\r\n"
"INFO\r\n"
"$7\r\n"
"Cluster\r\n";
static const char* const CLUSTER_SLOTS =
"*1\r\n"
"$13\r\n"
"CLUSTER SLOTS";

/*
 * Implemented in crc16.cpp
 */
extern uint16_t crc16(const char *buf, int len);

/* Copy from cluster.c
 *
 * We have 16384 hash slots. The hash slot of a given key is obtained
 * as the least significant 14 bits of the crc16 of the key.
 *
 * However if the key contains the {...} pattern, only the part between
 * { and } is hashed. This may be useful in the future to force certain
 * keys to be in the same node (assuming no resharding is in progress). */
static unsigned int keyHashSlot(const char *key, size_t keylen) {
    size_t s, e; /* start-end indexes of { and } */

    for (s = 0; s < keylen; s++)
        if (key[s] == '{') break;

    /* No '{' ? Hash the whole key. This is the base case. */
    if (s == keylen) return crc16(key,keylen) & 0x3FFF;

    /* '{' found? Check if we have the corresponding '}'. */
    for (e = s+1; e < keylen; e++)
        if (key[e] == '}') break;

    /* No '}' or nothing betweeen {} ? Hash the whole key. */
    if (e == keylen || e == s+1) return crc16(key,keylen) & 0x3FFF;

    /* If we are here there is both a { and a } on its right. Hash
     * what is in the middle between { and }. */
    return crc16(key+s+1,e-s-1) & 0x3FFF; // 0x3FFF == 16383
}

/*
 * class redis_client
 */
unsigned int redis_client::get_key_slot(const std::string& key)
{
    if (!key.empty())
    {
        return keyHashSlot(key.c_str(), key.size());
    }
    else
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        srandom(tv.tv_usec);
        return random() & 0x3FFF;
    }
}

redis_client::redis_client(const std::string host, uint16_t port)
: m_host(host)
, m_port(port)
, m_rcon(NULL)
, m_binitialization(false)
, m_cluster_mode(false)
, m_socket(NULL)
{
    init();
}

redis_client::~redis_client()
{
    clear();
}
bool redis_client::init()
{
    clear();

    m_socket = new socket_client();
    m_socket->connect_socket(m_host.c_str(), m_port);

    if (list_node()) {
        m_binitialization = true;
        return true;
    }
    else {
        return false;
    }
}

bool redis_client::list_node()
{
    redisContext* redis_context = connect_node(std::make_pair(m_host, m_port));
    if (NULL == redis_context) {
        return false;
    }
    m_rcon = redis_context;

    redisReply* redis_reply = (redisReply*)redisCommand(m_rcon, "INFO Cluster");
    if (redis_reply->type == REDIS_REPLY_STRING) {
        char* ch = strstr(redis_reply->str, ":");
        m_cluster_mode = (0 == atoi(ch+1)) ? false : true;
        freeReplyObject(redis_reply);
    }
    else {
        ERROR("Execute command fail! Command[INFO Cluster]");
        freeReplyObject(redis_reply);
        return false;
    }

    if (m_cluster_mode) {
        // 默认节点为slave节点，在parse_cluster_slots函数中会重新确定节点是否是master节点
        t_cluster_node* p_cluster_node = create_cluster_node(m_host, m_port, false, m_rcon);
        t_node_pair node_pair = std::make_pair(m_host, m_port);
        m_nodes.insert(std::make_pair(node_pair, p_cluster_node));

        redis_reply = (redisReply*)redisCommand(m_rcon, "CLUSTER SLOTS");
        parse_cluster_slots(redis_reply);
        freeReplyObject(redis_reply);
    }

    return true;
}

bool redis_client::list_node_new()
{
    socket_client* socket = connect_node(m_host, m_port);
    if (NULL == socket) {
        return false;
    }
    m_socket = socket;

    redis_reply* reply = run(INFO_CLUSTER);
    if (reply->get_type() == T_REDIS_REPLY_STRING) {
        std::string str = reply->get_string();
        int pos = str.find(":");
        m_cluster_mode = (0 == atoi(str.at(pos+1))) ? false : true;
        SAFE_DELETE(reply);
    }
    else {
        ERROR("Execute command fail! Command[INFO Cluster]");
        SAFE_DELETE(reply);
        return false;
    }

    if (m_cluster_mode) {
        // 默认节点为slave节点，在parse_cluster_slots函数中会重新确定节点是否是master节点
        t_cluster_node* p_cluster_node =
            create_cluster_node(m_host, m_port, false, NULL, m_socket);
        t_node_pair node_pair = std::make_pair(m_host, m_port);
        m_nodes.insert(std::make_pair(node_pair, p_cluster_node));

        reply = run(CLUSTER_SLOTS);
        if (!parse_cluster_slots(reply)) {
            SAFE_DELETE(reply);
            return false;
        }
        else {
            SAFE_DELETE(reply);
            return true;
        }
    }

    return true;
}

redisContext* redis_client::connect_node(const t_node_pair& node)
{
    redisContext* redis_context = redisConnect(node.first.c_str(), node.second);

    if (NULL == redis_context || 0 != redis_context->err) {
        ERROR("Can't connect to Redis at [%s:%d], errstr: %s",
            node.first.c_str(), node.second,
            redis_context ? redis_context->errstr : "Can't allocate redis context");
        redisFree(redis_context);
        return NULL;
    }
    else {
        return redis_context;
    }
}

socket_client* redis_client::connect_node(const std::string& host, uint16_t port)
{
    socket_client* socket = new socket_client();
    if (socket->connect_socket(host.c_str(), port) < 0) {
        ERROR("Can't connect to Redis at [%s:%u]", host.c_str(), port);
        SAFE_DELETE(socket);
        return NULL;
    }
    else {
        return socket;
    }
}

bool redis_client::parse_cluster_slots(redisReply * reply)
{
    if (NULL == reply) {
        return false;
    }

    if (reply->type != REDIS_REPLY_ARRAY || reply->elements <= 0) {
        ERROR("Reply error: reply-type(%s) is't array or elements(%d) <= 0.",
              REPLY_TYPE[reply->type], reply->elements);
        clear_slots();
        return false;
    }

    for (size_t i = 0; i < reply->elements; i ++) {
        redisReply* elem_slots = reply->element[i];
        if (elem_slots->type != REDIS_REPLY_ARRAY || elem_slots->elements < 3) {
            ERROR("Reply Error: reply-type(%s) is't array or elements(%d) < 3.",
                  REPLY_TYPE[reply->type], reply->elements);
            clear_slots();
            return false;
        }

        t_cluster_slots* slots = new t_cluster_slots;
        if (NULL == slots) {
            ERROR("new t_cluster_slots error!");
            clear_slots();
            return false;
        }

        // one slits region
        for (size_t idx = 0; idx < elem_slots->elements; idx ++) {
            if (idx == 0) { // begin of slots
                redisReply* elem_slots_begin = elem_slots->element[idx];
                if (elem_slots_begin->type != REDIS_REPLY_INTEGER) {
                    ERROR("Reply Type Error: reply-type(%s) isn't integer.");
                    clear_slots();
                    return false;
                }
                slots->begin = (int)(elem_slots_begin->integer);
            }
            else if (idx == 1) { // end of slots
                redisReply* elem_slots_end = elem_slots->element[idx];
                if (elem_slots_end->type != REDIS_REPLY_INTEGER) {
                    ERROR("Reply Type Error: reply-type(%s) isn't integer.");
                    clear_slots();
                    return false;
                }
                slots->end = (int)(elem_slots_end->integer);

                if (slots->begin > slots->end) {
                    ERROR("slots->begin(%d) > slots->end(%d)",
                          slots->begin, slots->end);
                    clear_slots();
                    return false;
                }
            }
            else { // cluster node
                redisReply* elem_node = elem_slots->element[idx];
                if (elem_node->type != REDIS_REPLY_ARRAY ||
                    elem_node->elements != 3) {
                    ERROR("Reply Error: reply-type(%s) is't array or elements(%d) != 3.",
                          REPLY_TYPE[reply->type], reply->elements);
                    clear_slots();
                    return false;
                }

                redisReply* elem_ip = elem_node->element[0];
                redisReply* elem_port = elem_node->element[1];

                if (elem_ip == NULL || elem_port ==NULL ||
                    elem_ip->type != REDIS_REPLY_STRING || 
                    elem_port->type != REDIS_REPLY_INTEGER) {
                    ERROR("Reply Type Error: elem_ip(%s), elem_port(%s)",
                        elem_ip == NULL ? "NULL" : REPLY_TYPE[elem_ip->type],
                        elem_port == NULL ? "NULL" : REPLY_TYPE[elem_port->type]);
                    clear_slots();
                    return false;
                }

                // insert new node, if node isn't in map
                t_cluster_node* p_cluster_node = NULL;
                t_node_pair node_pair = std::make_pair(elem_ip->str, elem_port->integer);
                t_cluster_node_map_iter it = m_nodes.find(node_pair);
                if (it == m_nodes.end()) {
                    p_cluster_node = 
                        create_cluster_node(elem_ip->str, elem_port->integer,
                                            idx == 2 ? true : false);
                    m_nodes.insert(std::make_pair(node_pair, p_cluster_node));
                }
                else {
                    p_cluster_node = it->second;
                    p_cluster_node->master = (idx == 2 ? true : false);
                }

                // this is master
                if (idx == 2) {
                    slots->node = p_cluster_node;
                }
            }
        }

        m_slots.push_front(slots);
    }

    return true;
}

bool redis_client::parse_cluster_slots(redis_reply * reply)
{
    if (NULL == reply) {
        return false;
    }

    if (reply->get_type() != T_REDIS_REPLY_ARRAY || reply->get_size() <= 0) {
        ERROR("Reply error: reply-type(%s) is't array or size(%d) <= 0.",
              REPLY_TYPE[reply->get_type()], reply->get_size());
        clear_slots();
        return false;
    }

    for (size_t i = 0; i < reply->get_size(); i ++) {
        const redis_reply* elem_slots = reply->get_element(i);
        if (elem_slots->get_type() != T_REDIS_REPLY_ARRAY
            || elem_slots->get_size() < 3) {
            ERROR("Reply Error: reply-type(%s) is't array or size(%d) < 3.",
                  REPLY_TYPE[reply->get_type()], reply->get_size());
            clear_slots();
            return false;
        }

        t_cluster_slots* slots = new t_cluster_slots;
        if (NULL == slots) {
            ERROR("new t_cluster_slots error!");
            clear_slots();
            return false;
        }

        // one slits region
        for (size_t idx = 0; idx < elem_slots->get_size(); idx ++) {
            if (idx == 0) { // begin of slots
                redis_reply* elem_slots_begin = elem_slots->get_element(idx);
                if (elem_slots_begin->get_type() != T_REDIS_REPLY_INTEGER) {
                    ERROR("Reply Type Error: reply-type(%s) isn't integer.");
                    clear_slots();
                    return false;
                }
                slots->begin = (int)(elem_slots_begin->get_integer());
            }
            else if (idx == 1) { // end of slots
                redis_reply* elem_slots_end = elem_slots->get_element(idx);
                if (elem_slots_end->get_type() != T_REDIS_REPLY_INTEGER) {
                    ERROR("Reply Type Error: reply-type(%s) isn't integer.");
                    clear_slots();
                    return false;
                }
                slots->end = (int)(elem_slots_end->get_integer());

                if (slots->begin > slots->end) {
                    ERROR("slots->begin(%d) > slots->end(%d)",
                          slots->begin, slots->end);
                    clear_slots();
                    return false;
                }
            }
            else { // cluster node
                redis_reply* elem_node = elem_slots->get_element(idx);
                if (elem_node->get_type() != T_REDIS_REPLY_ARRAY ||
                    elem_node->get_size() != 3) {
                    ERROR("Reply Error: reply-type(%s) is't array or size(%d) != 3.",
                          REPLY_TYPE[reply->get_type()], reply->get_size());
                    clear_slots();
                    return false;
                }

                redis_reply* elem_ip = elem_node->get_element(0);
                redis_reply* elem_port = elem_node->get_element(1);

                if (elem_ip == NULL || elem_port == NULL ||
                    elem_ip->get_type() != T_REDIS_REPLY_STRING || 
                    elem_port->get_type() != T_REDIS_REPLY_INTEGER) {
                    ERROR("Reply Type Error: elem_ip(%s), elem_port(%s)",
                        elem_ip == NULL ? "NULL" : REPLY_TYPE[elem_ip->get_type()],
                        elem_port == NULL ? "NULL" : REPLY_TYPE[elem_port->get_type()]);
                    clear_slots();
                    return false;
                }

                // insert new node, if node isn't in map
                t_cluster_node* p_cluster_node = NULL;
                t_node_pair node_pair =
                    std::make_pair(elem_ip->get_string(), elem_port->get_integer());
                t_cluster_node_map_iter it = m_nodes.find(node_pair);
                if (it == m_nodes.end()) {
                    p_cluster_node = 
                        create_cluster_node(elem_ip->str, elem_port->integer,
                                            idx == 2 ? true : false);
                    m_nodes.insert(std::make_pair(node_pair, p_cluster_node));
                }
                else {
                    p_cluster_node = it->second;
                    p_cluster_node->master = (idx == 2 ? true : false);
                }

                // this is master
                if (idx == 2) {
                    slots->node = p_cluster_node;
                }
            }
        }

        m_slots.push_front(slots);
    }

    return true;
}

void redis_client::clear()
{
    clear_nodes();
    clear_slots();
}

void redis_client::clear_nodes()
{
    if (!m_cluster_mode) {
        redisFree(m_rcon);
        if (m_socket) {
            m_socket->close_socket();
            SAFE_DELETE(m_socket);
        }
    }

    t_cluster_node_map_iter it = m_nodes.begin();
    for (; it != m_nodes.end(); ++it) {
        t_cluster_node* node = it->second;
        redisContext* redis_context = node->con;
        redisFree(redis_context);
        SAFE_DELETE(node->socket);
        delete node;
    }
    m_nodes.clear();
}

void redis_client::clear_slots()
{
    t_slots_list_iter it = m_slots.begin();
    for (; it != m_slots.end(); ++it) {
        SAFE_DELETE(*it);
    }
    m_slots.clear();
}

redis_client::t_cluster_node*
redis_client::create_cluster_node(const std::string host,
                                  uint16_t port,
                                  bool master /*= false*/,
                                  redisContext * con /*= NULL*/,
                                  socket_client * socket /*= NULL*/)
{
    t_cluster_node* node = new t_cluster_node;

    node->host = host;
    node->port = port;
    node->master = master;
    node->con = con;
    node->socket = socket;

    return node;
}

bool redis_client::is_normal_context(redisContext * rcon)
{
    // 如果给定redisContext不可用，则释放掉
    if(rcon == NULL || rcon->err) {
        redisFree(rcon);
        rcon = NULL;
        return false;
    }
    return true;
}

redisContext* redis_client::get_normal_context()
{
    if (m_cluster_mode) { // 集群模式
        return get_normal_context_with_cluster_mode();
    }
    else { // 单点模式
        return get_normal_context_with_singleton_mode();
    }
}

redisContext* redis_client::get_normal_context_with_cluster_mode()
{
    if (!m_cluster_mode) {
        WARN("This is in singleton mode!");
        return NULL;
    }

    if (is_normal_context(m_rcon)) { // 1.当前连接有效，返回当前连接
        return m_rcon;
    }

    // 2.当前连接无效时，尝试重新连接当前节点
    redisContext* redis_context = connect_node(std::make_pair(m_host, m_port));
    // 3.重新连接成功时，保存新的连接并返回该连接
    if (is_normal_context(redis_context)) {
        m_rcon = redis_context;
        t_cluster_node_map_iter it = m_nodes.find(std::make_pair(m_host, m_port));
        if (it != m_nodes.end()) { // 保存新的连接
            it->second->con = m_rcon;
        }
        else { // 正常情况，不应该走到该分支
            ERROR("%s:%d isn't in cluster!", m_host.c_str(), m_port);
        }
        return m_rcon;
    }

    // 4.重新连接失败时，遍历集群，查找连接有效的节点
    t_cluster_node_map_iter it = m_nodes.begin();
    for (; it != m_nodes.end(); ++it) {
        t_cluster_node* p_cluster_node = it->second;
        // 5.若该连接无效，尝试重新连接
        // PS:重新连接依然可能出现链接无效的情况
        if (!is_normal_context(p_cluster_node->con)) {
            redis_context = connect_node(it->first);
            if (is_normal_context(redis_context)) {
                p_cluster_node->con = redis_context;
            }
        }

        // 6.如果连接成功，更新当前节点信息，并返回
        if (p_cluster_node->con) {
            m_host = p_cluster_node->host;
            m_port = p_cluster_node->port;
            m_rcon = p_cluster_node->con;
            
            return m_rcon;
        }
    }

    // 7.所有节点都无法正常连接时，返回空
    ERROR("All cluster node can't connect!");
    return NULL;
}

redisContext* redis_client::get_normal_context_with_singleton_mode()
{
    if (m_cluster_mode) {
        WARN("This is in cluster mode!");
        return NULL;
    }

    if (is_normal_context(m_rcon)) { // 1.当前连接有效，返回当前连接
        return m_rcon;
    }
    else { // 2.当前连接无效时，尝试重新连接当前节点
        redisContext* redis_context = connect_node(std::make_pair(m_host, m_port));
        if (is_normal_context(redis_context)) {
            m_rcon = redis_context;
        }
        else { // 正常情况，不应该走到该分支
            m_rcon = NULL;
        }
        return m_rcon;
    }
}

redisContext* redis_client::get_redis_context()
{
    return get_normal_context();
}

redisContext* 
redis_client::get_redis_context(const std::string host, uint16_t port)
{
    if (m_cluster_mode) {
        t_cluster_node_map_iter it = m_nodes.find(std::make_pair(host, port));
        if (it == m_nodes.end()) {
            ERROR("%s:%d isn't in cluster!", host.c_str(), port);
            return NULL;
        }
        else {
            t_cluster_node* node = it->second;
            if (node->con == NULL) {                
                redisContext* redis_context =
                    connect_node(std::make_pair(node->host, node->port));
                node->con = redis_context;
            }

            if (node->con != NULL) {
                m_host = node->host;
                m_port = node->port;
                m_rcon = node->con;
            }

            return m_rcon;
        }
    }
    else { // single-one
        if (host == m_host && port == m_port) {
            return m_rcon;
        }
        else {
            return NULL;
        }
    }
}

redisContext* redis_client::get_redis_context_by_slot(int slot)
{
    if (!m_cluster_mode) {
        return m_rcon;
    }
    else {
        t_slots_list_iter it = m_slots.begin();
        for (; it != m_slots.end(); ++it) {
            if (((*it)->begin > slot) || ((*it)->end < slot)) {
                continue;
            }

            t_cluster_node* node = (*it)->node;
            if (node->con == NULL) {
                redisContext* redis_context =
                    connect_node(std::make_pair(node->host, node->port));
                node->con = redis_context;
            }
            if (node->con != NULL) {
                m_host = node->host;
                m_port = node->port;
                m_rcon = node->con;
            }
            return node->con;
        }

        return NULL;
    }
}

redisContext* redis_client::get_redis_context_by_key(std::string key)
{
    if (m_cluster_mode) {
        return get_redis_context_by_slot(get_key_slot(key));
    }
    else {
        return m_rcon;
    }
}


redis_reply* redis_client::run(const std::string& request)
{
    if (!request.empty() && m_socket->send_msg(request.c_str()) == -1) {
        ERROR("send to redis(%s:%d) error, req: %s",
              m_host.c_str(), m_port, request.c_str());
        return NULL;
    }

    return get_redis_object();
}

void redis_client::put_data(redis_reply * rr, const std::string & data)
{
    int pos = data.find('\r');
    if (pos >= 0) { // 去除末尾的\r\n
        rr->put(data.substr(0, pos));
    }
    else {
        rr->put(data);
    }
}

redis_reply* redis_client::process_line_item(t_redis_reply type)
{
    m_buff.clear();
    if (m_socket->read_line(m_buff) <= 0) {
        ERROR("read line from redis(%s:%d) error", m_host.c_str(), m_port);
        return NULL;
    }

    redis_reply* rr = new redis_reply();
    rr->set_type(type);

    put_data(rr, m_buff);
    return rr;
}

redis_reply* redis_client::get_redis_error()
{
    return process_line_item(T_REDIS_REPLY_ERROR);
}

redis_reply* redis_client::get_redis_status()
{
    return process_line_item(T_REDIS_REPLY_STATUS);
}

redis_reply* redis_client::get_redis_integer()
{
    return process_line_item(T_REDIS_REPLY_INTEGER);
}

redis_reply* redis_client::get_redis_string()
{
    redis_reply* rr = NULL;
    m_buff.clear();
    if (m_socket->read_line(m_buff) <= 0) {
        ERROR("read line from redis(%s:%d) error", m_host.c_str(), m_port);
        return NULL;
    }

    int len = atoi(m_buff.c_str());
    if (len < 0) {
        rr = new redis_reply();
        rr->set_type(T_REDIS_REPLY_NIL);
    }
    else {
        m_buff.clear();
        if (len > 0 && m_socket->read_line(m_buff) <= 0) {
            ERROR("read line from redis(%s:%d) error", m_host.c_str(), m_port);
            return NULL;
        }
        rr = new redis_reply();
        rr->set_type(T_REDIS_REPLY_STRING);
        put_data(rr, m_buff);
    }
    return rr;
}

redis_reply* redis_client::get_redis_array()
{
    redis_reply* rr = NULL;
    m_buff.clear();
    if (m_socket->read_line(m_buff) <= 0) {
        ERROR("read line from redis(%s:%d) error", m_host.c_str(), m_port);
        return NULL;
    }

    int count = atoi(m_buff.c_str());
    if (count <= 0) {
        rr = new redis_reply();
        rr->set_type(T_REDIS_REPLY_NIL);
    }
    else {
        rr = new redis_reply();
    	rr->set_type(T_REDIS_REPLY_ARRAY);
        for (int i = 0; i < count; i++) {
            redis_reply* element = get_redis_object();
            if (element == NULL) {
                delete rr;  // 释放整个数组, 防止内存泄漏
                return NULL;
            }
            rr->put(element);
        }
    }
    return rr;
}

redis_reply* redis_client::get_redis_object()
{
    char ch;
    if (m_socket->recv_msg(&ch, 1) == -1) {
        return NULL;
    }

    switch (ch) {
        case '-':   // ERROR
            return get_redis_error();
        case '+':   // STATUS
            return get_redis_status();
        case ':':   // INTEGER
            return get_redis_integer();
        case '$':   // STRING
            return get_redis_string();
        case '*':   // ARRAY
            return get_redis_array();
        default:    // INVALID
            return NULL;
    }
}


