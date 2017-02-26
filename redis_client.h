#ifndef __REDIS_H__
#define __REDIS_H__

#include <string>
#include <map>
#include <list>

#include <hiredis.h>

uint16_t crc16(const char *buf, int len);


class redis_client
{
public:
    typedef struct cluster_node {
        std::string host;
        uint16_t port;
        bool master;
        redisContext* con;
    } t_cluster_node;
    typedef std::pair<std::string, uint16_t> t_node_pair;
    typedef std::map<t_node_pair, t_cluster_node*> t_cluster_node_map;
    typedef t_cluster_node_map::iterator t_cluster_node_map_iter;

    typedef struct cluster_slots {
        int begin;
        int end;
        t_cluster_node* node;
    } t_cluster_slots;
    typedef std::list<t_cluster_slots*> t_slots_list;
    typedef t_slots_list::iterator t_slots_list_iter;

public:
    redis_client(const std::string host, uint16_t port);
    ~redis_client();

    redisContext* get_redis_context_by_key(std::string key);

private:
    void init();
    void list_node();
    redisContext* connect_node(const t_node_pair& node);
    bool parse_cluster_slots(redisReply* reply);
    void clear();
    void clear_nodes();
    void clear_slots();
    redisContext* get_redis_context_by_slot(int slot);

    t_cluster_node* create_cluster_node(const std::string host, uint16_t port,
                                        bool master = false,
                                        redisContext* con = NULL);


private:
    std::string m_host;
    uint16_t m_port;

private:
    bool m_cluster_mode;
    t_cluster_node_map m_nodes;
    t_slots_list m_slots;                   // slots of master
};

#endif /* __REDIS_H__ */
