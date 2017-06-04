#if 0
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "RedisClient.h"
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
"*2\r\n"
"$7\r\n"
"CLUSTER\r\n"
"$5\r\n"
"SLOTS\r\n";

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

redis_cluster::redis_cluster(const std::string host, uint16_t port)
: m_binitialization(false)
, m_host(host)
, m_port(port)
, m_socket(NULL)
, m_cluster_mode(false)
{
    init();
}

redis_cluster::~redis_cluster()
{
    clear();
}
bool redis_cluster::init()
{
    if (list_node()) {
        m_binitialization = true;
        return true;
    }
    else {
        ERROR("Initialize fail!");
        clear();
        return false;
    }
}

void redis_cluster::clear()
{
    if (!m_cluster_mode) {
        if (m_socket) {
            m_socket->close_socket();
            SAFE_DELETE(m_socket);
        }
    }
    else {
        t_cluster_node_map_iter map_it = m_nodes.begin();
        for (; map_it != m_nodes.end(); ++map_it) {
            t_cluster_node* node = map_it->second;
            if (node) {
                if (node->socket) {
                    node->socket->close_socket();
                    delete node->socket;
                }
                delete node;
            }
        }
        m_nodes.clear();

        t_slots_list_iter list_it = m_slots.begin();
        for (; list_it != m_slots.end(); ++list_it) {
            SAFE_DELETE(*list_it);
        }
        m_slots.clear();
    }
}

bool redis_cluster::list_node()
{
    socket_client* socket = connect_node(m_host, m_port);
    if (NULL == socket) {
        return false;
    }
    m_socket = socket;

    redisReply* reply = run(INFO_CLUSTER);
    if (reply && reply->get_type() == REDIS_REPLY_STRING) {
        std::string str = reply->get_string();
        int pos = str.find(":");
        m_cluster_mode = ('0' == str.at(pos+1)) ? false : true;
        SAFE_DELETE(reply);
    }
    else {
        ERROR("Execute command fail! Command[INFO Cluster], %s",
              parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return false;
    }

    if (m_cluster_mode) {
        // 默认节点为slave节点, 在parse_cluster_slots函数中会重新确定节点是否是master节点
        t_cluster_node* p_cluster_node =
            create_cluster_node(m_host, m_port, false, m_socket);
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

socket_client* redis_cluster::connect_node(const std::string& host, uint16_t port)
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

std::string redis_cluster::parse_reply(const redisReply* reply)
{
    if (reply == NULL) {
        return "Reply is NULL";
    }

    std::string result("Reply Type: ");
    result += REPLY_TYPE[reply->get_type()];
    switch(reply->get_type()) {
        case REDIS_REPLY_STATUS:
            return result + ", Status: " + reply->get_status();
        case REDIS_REPLY_NIL:
            return result;
        case REDIS_REPLY_STRING:
            return result + ", String: " + reply->get_string();
        case REDIS_REPLY_ERROR:
            return result + ", Errstr: " + reply->get_error();
        case REDIS_REPLY_INTEGER:
            return result + ", Integer: "
                    + TO_STRING(reply->get_integer());
        case REDIS_REPLY_ARRAY:
            return result + ", Array Elements:"
                    + TO_STRING(reply->get_size());
        default:
            return "Unkonw Type";
    }
}

bool redis_cluster::parse_cluster_slots(redisReply * reply)
{
    if (NULL == reply) {
        return false;
    }

    if (reply->get_type() != REDIS_REPLY_ARRAY || reply->get_size() <= 0) {
        ERROR("Reply Error: reply-type is't array or size(%d) <= 0. %s",
              reply->get_size(), parse_reply(reply).c_str());
        return false;
    }

    for (size_t i = 0; i < reply->get_size(); i ++) {
        const redisReply* elem_slots = reply->get_element(i);
        if (elem_slots->get_type() != REDIS_REPLY_ARRAY
            || elem_slots->get_size() < 3) {
            ERROR("Reply Error: reply-type is't array or size(%d) < 3. %s",
                  reply->get_size(), parse_reply(reply).c_str());
            return false;
        }

        t_cluster_slots* slots = new t_cluster_slots;
        if (NULL == slots) {
            ERROR("new t_cluster_slots error!");
            return false;
        }

        // one slits region
        for (size_t idx = 0; idx < elem_slots->get_size(); idx ++) {
            if (idx == 0) { // begin of slots
                const redisReply* elem_slots_begin = elem_slots->get_element(idx);
                if (elem_slots_begin->get_type() != REDIS_REPLY_INTEGER) {
                    ERROR("Reply Type Error: reply-type isn't integer. %s",
                          parse_reply(reply).c_str());
                    return false;
                }
                slots->begin = (int)(elem_slots_begin->get_integer());
            }
            else if (idx == 1) { // end of slots
                const redisReply* elem_slots_end = elem_slots->get_element(idx);
                if (elem_slots_end->get_type() != REDIS_REPLY_INTEGER) {
                    ERROR("Reply Type Error: reply-type isn't integer. %s",
                          parse_reply(reply).c_str());
                    return false;
                }
                slots->end = (int)(elem_slots_end->get_integer());

                if (slots->begin > slots->end) {
                    ERROR("slots->begin(%d) > slots->end(%d)",
                          slots->begin, slots->end);
                    return false;
                }
            }
            else { // cluster node
                const redisReply* elem_node = elem_slots->get_element(idx);
                if (elem_node->get_type() != REDIS_REPLY_ARRAY ||
                    elem_node->get_size() != 3) {
                    ERROR("Reply Error: reply-type is't array or size(%d) != 3. %s",
                          reply->get_size(), parse_reply(reply).c_str());
                    return false;
                }

                const redisReply* elem_ip = elem_node->get_element(0);
                const redisReply* elem_port = elem_node->get_element(1);

                if (elem_ip == NULL || elem_port == NULL ||
                    elem_ip->get_type() != REDIS_REPLY_STRING || 
                    elem_port->get_type() != REDIS_REPLY_INTEGER) {
                    ERROR("Reply Type Error: elem_ip(%s), elem_port(%s)",
                        elem_ip == NULL ? "NULL" : REPLY_TYPE[elem_ip->get_type()],
                        elem_port == NULL ? "NULL" : REPLY_TYPE[elem_port->get_type()]);
                    return false;
                }

                // insert new node, if node isn't in map
                t_cluster_node* p_cluster_node = NULL;
                t_node_pair node_pair =
                    std::make_pair(elem_ip->get_string(), elem_port->get_integer());
                t_cluster_node_map_iter it = m_nodes.find(node_pair);
                if (it == m_nodes.end()) {
                    p_cluster_node = create_cluster_node(elem_ip->get_string(),
                        elem_port->get_integer(), idx == 2 ? true : false);
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

redis_cluster::t_cluster_node*
redis_cluster::create_cluster_node(const std::string host,
                                  uint16_t port,
                                  bool master /*= false*/,
                                  socket_client * socket /*= NULL*/)
{
    t_cluster_node* node = new t_cluster_node;

    node->host = host;
    node->port = port;
    node->master = master;
    node->socket = socket;

    return node;
}

void redis_cluster::redirect(int slot)
{
    if (!m_cluster_mode) {
        return;
    }

    t_slots_list_iter it = m_slots.begin();
    for (; it != m_slots.end(); ++it) {
        if (((*it)->begin > slot) || ((*it)->end < slot)) {
            continue;
        }

        t_cluster_node* node = (*it)->node;
        if (node->socket == NULL) {
            socket_client* socket = connect_node(node->host, node->port);
            node->socket = socket;
        }
        if (node->socket != NULL) {
            m_host = node->host;
            m_port = node->port;
            m_socket = node->socket;
        }
    }
}

unsigned int redis_cluster::get_key_slot(const std::string& key)
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

void redis_cluster::set_hash_slot(const std::string & key)
{
    // 非集群模式不需要设置哈希槽
    if (!m_cluster_mode) {
        return;
    }
    int slot = get_key_slot(key);
    redirect(slot);
}

redisReply* redis_cluster::run(const std::string& request)
{
    if (m_socket == NULL) {
        ERROR("m_socket is NULL");
        return NULL;
    }

    if (!request.empty() &&
        m_socket->write(request.c_str(), request.size()) == -1) {
        ERROR("send to redis(%s:%d) error, req: %s",
              m_host.c_str(), m_port, request.c_str());
        return NULL;
    }

    return get_redis_object();
}

redisReply* redis_cluster::process_line_item(t_redis_reply type)
{
    if (m_socket == NULL) {
        ERROR("m_socket is NULL");
        return NULL;
    }

    std::string buff = m_socket->read_line();
    if (buff.size() <= 0) {
        ERROR("read line from redis(%s:%d) error", m_host.c_str(), m_port);
        return NULL;
    }

    redisReply* rr = new redisReply();
    rr->set_type(type);
    rr->put(buff);

    return rr;
}

redisReply* redis_cluster::get_redis_error()
{
    return process_line_item(REDIS_REPLY_ERROR);
}

redisReply* redis_cluster::get_redis_status()
{
    return process_line_item(REDIS_REPLY_STATUS);
}

redisReply* redis_cluster::get_redis_integer()
{
    return process_line_item(REDIS_REPLY_INTEGER);
}

redisReply* redis_cluster::get_redis_string()
{
    if (m_socket == NULL) {
        ERROR("m_socket is NULL");
        return NULL;
    }

    std::string str_buff = m_socket->read_line();
    if (str_buff.size() <= 0) {
        ERROR("read line from redis(%s:%d) error", m_host.c_str(), m_port);
        return NULL;
    }

    redisReply* rr = NULL;
    int len = atoi(str_buff.c_str());
    if (len < 0) {
        rr = new redisReply();
        rr->set_type(REDIS_REPLY_NIL);
    }
    else {
        char* buff = new char[len + 1];
        if (len > 0 && m_socket->read(buff, len) <= 0) {
            ERROR("read line from redis(%s:%d) error", m_host.c_str(), m_port);
            return NULL;
        }
        buff[len] = 0;
        rr = new redisReply();
        rr->set_type(REDIS_REPLY_STRING);
        rr->put(buff, len);

        // 读取\r\n
        str_buff = m_socket->read_line();
    }
    return rr;
}

redisReply* redis_cluster::get_redis_array()
{
    if (m_socket == NULL) {
        ERROR("m_socket is NULL");
        return NULL;
    }

    std::string str_buff = m_socket->read_line();
    if (str_buff.size() <= 0) {
        ERROR("read line from redis(%s:%d) error", m_host.c_str(), m_port);
        return NULL;
    }

    redisReply* rr = NULL;
    int count = atoi(str_buff.c_str());
    if (count <= 0) {
        rr = new redisReply();
        rr->set_type(REDIS_REPLY_NIL);
    }
    else {
        rr = new redisReply();
    	rr->set_type(REDIS_REPLY_ARRAY);
        for (int i = 0; i < count; i++) {
            redisReply* element = get_redis_object();
            if (element == NULL) {
                SAFE_DELETE(rr);  // 释放整个数组, 防止内存泄漏
                return NULL;
            }
            rr->put(element);
        }
    }
    return rr;
}

redisReply* redis_cluster::get_redis_object()
{
    if (m_socket == NULL) {
        ERROR("m_socket is NULL");
        return NULL;
    }

    char ch = m_socket->read_byte();
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
#endif /* 2017-06-04 15:57:19 chen.tengfei */

