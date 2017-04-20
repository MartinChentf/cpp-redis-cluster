#ifndef __REDIS_H__
#define __REDIS_H__

#include <stdint.h>
#include <string>
#include <map>
#include <list>

#include "redis_reply.h"
class socket_client;

class redis_client
{
public:
    typedef struct cluster_node {
        std::string host;       // IP
        uint16_t port;          // port
        bool master;            // redis-server��master�ڵ�
        socket_client* socket;  // ��redis-server�ڵ����ӵ�socket
    } t_cluster_node;
    typedef std::pair<std::string, uint16_t> t_node_pair;
    typedef std::map<t_node_pair, t_cluster_node*> t_cluster_node_map;
    typedef t_cluster_node_map::iterator t_cluster_node_map_iter;

    // ��ϣ������
    typedef struct cluster_slots {
        int begin;
        int end;
        t_cluster_node* node; // ������Ĺ�ϣ�����ڵ�redis-server�ڵ�
    } t_cluster_slots;
    typedef std::list<t_cluster_slots*> t_slots_list;
    typedef t_slots_list::iterator t_slots_list_iter;

public:
    redis_client(const std::string host, uint16_t port);
    ~redis_client();

    /**
     * @description
     *   ��ǰredis_client�Ƿ����ӵ���redis��Ⱥ
     * @return {bool} true-��Ⱥ; false-�Ǽ�Ⱥ
     * @author chen.tengfei
     * @date 2017-04-20
     */
    bool cluster() { return m_cluster_mode; }

    /**
     * @description
     *   ִ�����������, �����ؾ���������redis_server��Ӧ��
     * @param [IN] request {const std::string&}
     *   ��������, �����ʽ��ѭRedisЭ��淶
     * @return {redis_reply*} �������redis_server��Ӧ��, ����ֵ�ɵ��÷������ͷ�
     * @author chen.tengfei
     * @date 2017-04-20
     */
    redis_reply* run(const std::string& request);

    /**
     * @description
     *   ����keyֵ�����ϣ��, ʹredis_client�ض��򵽹�ϣ�����ڵ�redis_server
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
    // ���ӵ�ָ��redis_server�ڵ�, ʧ�ܻ���󷵻ؿ�
    socket_client* connect_node(const std::string& host, uint16_t port);
    bool parse_cluster_slots(redis_reply* reply);
    t_cluster_node* create_cluster_node(const std::string host,
        uint16_t port, bool master = false, socket_client* socket = NULL);

/******************************************************************************/
    unsigned int get_key_slot(const std::string& key);
    void redirect(int slot);

    void put_data(redis_reply* rr, const std::string& data);
    redis_reply* process_line_item(t_redis_reply type);
    redis_reply* get_redis_object();
    redis_reply* get_redis_error();
    redis_reply* get_redis_status();
    redis_reply* get_redis_integer();
    redis_reply* get_redis_string();
    redis_reply* get_redis_array();

private:
    std::string m_buff;

private:
    bool m_binitialization;

    // ���浱ǰ����redis_server�ڵ��host��IP��socket_client��Ϣ
    // for both cluster mode and singleton mode
    std::string m_host;
    uint16_t m_port;
    socket_client* m_socket;

private: // for cluster mode
    bool m_cluster_mode;
    t_cluster_node_map m_nodes; // cluster node
    t_slots_list m_slots;       // slots of master
};

#endif /* __REDIS_H__ */
