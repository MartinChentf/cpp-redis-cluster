#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "redis_client.h"
#include "redis_log.h"
#include "redis_helper.h"

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

    if (list_node()) {
        m_binitialization = true;
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

void redis_client::clear()
{
    clear_nodes();
    clear_slots();
}

void redis_client::clear_nodes()
{
    if (!m_cluster_mode) {
        redisFree(m_rcon);
    }

    t_cluster_node_map_iter it = m_nodes.begin();
    for (; it != m_nodes.end(); ++it) {
        t_cluster_node* node = it->second;
        redisContext* redis_context = node->con;
        redisFree(redis_context);
        delete node;
    }
    m_nodes.clear();
}

void redis_client::clear_slots()
{
    t_slots_list_iter it = m_slots.begin();
    for (; it != m_slots.end(); ++it) {
        delete *it;
    }
    m_slots.clear();
}

redis_client::t_cluster_node*
redis_client::create_cluster_node(const std::string host, uint16_t port,
                                  bool master /*= false*/,
                                  redisContext * con /*= NULL*/)
{
    t_cluster_node* node = new t_cluster_node;

    node->host = host;
    node->port = port;
    node->master = master;
    node->con = con;

    return node;
}

redisContext* redis_client::get_normal_context()
{
    // 尝试连接当前节点
    redisContext* redis_context = connect_node(std::make_pair(m_host, m_port));
    if (redis_context) {
        return redis_context;
    }

    if (!m_cluster_mode) {
        return NULL;
    }
    else {
        // 遍历集群，查找可正常连接的节点
        t_cluster_node_map_iter it = m_nodes.begin();
        for (; it != m_nodes.end(); ++it) {
            t_cluster_node* p_cluster_node = it->second;
            // 如果节点为空或有错误标识，重新连接该节点
            if (p_cluster_node->con == NULL || p_cluster_node->con->err) {
                redisFree(p_cluster_node->con); // 释放旧连接
                redis_context = connect_node(it->first);
                p_cluster_node->con = redis_context;
            }
            else {
                redis_context = p_cluster_node->con;
            }

            if (redis_context) { // 找到正常的连接，返回
                return redis_context;
            }
        }
        // 
        return NULL;
    }
}

redisContext* redis_client::get_redis_context()
{
    if (m_rcon == NULL || m_rcon->err) {
        redisFree(m_rcon);
        redisContext* redis_context = connect_node(std::make_pair(m_host, m_port));
        if (m_rcon == NULL) {
            
        }
    }
return m_rcon;

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

