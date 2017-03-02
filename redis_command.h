#ifndef __REFID_COMMAND_H__
#define __REFID_COMMAND_H__

#include <string>
#include <vector>

#include "hiredis.h"

class redis_client;

class redis_command
{
public:
    redis_command(redis_client* client);
    virtual ~redis_command(){}

    void set_client(redis_client* client);

protected:
    void hash_slots(std::string key);
    std::string build_command(const char *format, ...);
    redisReply* run_command();

    bool check_status();
    std::string get_string();
    std::string get_string_or_nil();
    long long get_integer64();
    int get_integer32();
    bool get_array(std::vector<std::string>* result);

private:
    std::string parse_reply(redisReply* reply);

    bool is_normal_context(); // �жϵ�ǰ�����Ƿ���Ч
    void confirm_redis_context();   // ȷ��Client�Ƿ��Ѿ�������redis, ���ܳ�������ʧ�ܵ����

private:
    redis_client* m_client;
    redisContext* m_rcon;
    std::string m_command;
    int m_slot;
};

#endif /* __REFID_COMMAND_H__ */
