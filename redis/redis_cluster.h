/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @file   redis_cluster.h
 * @brief 
 * @version 1.0
 * @bug none
 * @warning none
 * @other none
 * @author chen.tengfei
 * @data   2017-05-20 15:10:06
 */
#if 0
#ifndef __REDIS_CLUSTER_H__
#define __REDIS_CLUSTER_H__

#include <stdint.h>
#include <string>
#include <map>
#include <list>

#include "redisReply.h"
class socket_client;

class redis_cluster
{
public:
    typedef struct cluster_node {
        std::string host;       // IP
        uint16_t port;          // port
        bool master;            // redis-server是master节点
        socket_client* socket;  // 与redis-server节点连接的socket
    } t_cluster_node;
    typedef std::pair<std::string, uint16_t> t_node_pair;
    typedef std::map<t_node_pair, t_cluster_node*> t_cluster_node_map;
    typedef t_cluster_node_map::iterator t_cluster_node_map_iter;

    // 哈希槽区间
    typedef struct cluster_slots {
        int begin;
        int end;
        t_cluster_node* node; // 该区间的哈希槽所在的redis-server节点
    } t_cluster_slots;
    typedef std::list<t_cluster_slots*> t_slots_list;
    typedef t_slots_list::iterator t_slots_list_iter;

public:
    redis_cluster(const std::string host, uint16_t port);
    ~redis_cluster();

    /**
     * @description
     *   当前 RedisClient 是否连接的是redis集群
     * @return {bool} true-集群; false-非集群
     * @author chen.tengfei
     * @date 2017-04-20
     */
    bool cluster() { return m_cluster_mode; }

    /**
     * @description
     *   执行请求的命令, 并返回经过解析的redis_server的应答
     * @param [IN] request {const std::string&}
     *   请求命令, 命令格式遵循Redis协议规范
     * @return {redisReply*} 解析后的redis_server的应答, 返回值由调用方负责释放
     * @author chen.tengfei
     * @date 2017-04-20
     */
    redisReply* run(const std::string& request);

    /**
     * @description
     *   根据key值计算哈希槽, 使 RedisClient 重定向到哈希槽所在的redis_server
     * @param [IN] key {const std::string&} 
     * @return {void} 
     * @author chen.tengfei
     * @date 2017-04-19
     */
    void set_hash_slot(const std::string& key);

private:    
    bool init();
    void clear();
    bool list_node();
    // 连接到指定redis_server节点, 失败或错误返回空
    socket_client* connect_node(const std::string& host, uint16_t port);
    std::string parse_reply(const redisReply* reply);
    bool parse_cluster_slots(redisReply* reply);
    t_cluster_node* create_cluster_node(const std::string host,
        uint16_t port, bool master = false, socket_client* socket = NULL);

/******************************************************************************/
    unsigned int get_key_slot(const std::string& key);
    void redirect(int slot);

    redisReply* process_line_item(t_redis_reply type);
    redisReply* get_redis_object();
    redisReply* get_redis_error();
    redisReply* get_redis_status();
    redisReply* get_redis_integer();
    redisReply* get_redis_string();
    redisReply* get_redis_array();

private:
    bool m_binitialization;

    // 保存当前连接redis_server节点的host、IP和socket_client信息
    // for both cluster mode and singleton mode
    std::string m_host;
    uint16_t m_port;
    socket_client* m_socket;

private: // for cluster mode
    bool m_cluster_mode;
    t_cluster_node_map m_nodes; // cluster node
    t_slots_list m_slots;       // slots of master
};

#endif /* __REDIS_CLUSTER_H__ */
#endif /* 2017-06-04 15:56:59 chen.tengfei */
