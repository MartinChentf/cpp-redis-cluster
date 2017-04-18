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
    
    void build_request(const std::vector<std::string>& argv);
    redis_reply* run();
    
///////////////////////////////////////////////////////////////////////

    bool check_status();
    int check_status_or_nil();

    std::string get_string(bool* success = NULL);
    bool get_string(std::string* result);
    bool get_string(std::string& result);
    std::string get_string_or_nil(bool* success = NULL);
    int get_string_or_nil(std::string& result);

    long long get_integer64(bool* success = NULL);
    bool get_integer64(long long& result);
    bool get_integer64(long long* result);
    int get_integer32(bool* success = NULL);
    long long get_integer64_or_nil(bool * success = NULL);
    int get_integer32_or_nil(bool * success = NULL);

    bool get_array(std::vector<std::string*>& result);
    bool get_array(std::vector<std::string>& result);
    int get_array_or_nil(std::vector<std::string>& result);
    int get_cursor_array(std::vector<std::string>* result);

private:
    std::string parse_reply(redisReply* reply);

    bool is_normal_context(); // 判断当前连接是否有效
    void confirm_redis_context();   // 确认Client是否已经连接上redis, 可能出现连接失败的情况

private:
    redis_client* m_client;
    redisContext* m_rcon;
    std::string m_command;
    int m_slot;

private:
    std::string m_request_buf;
};

#endif /* __REFID_COMMAND_H__ */
