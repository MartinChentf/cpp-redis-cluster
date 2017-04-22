#ifndef __REDIS_COMMAND_H__
#define __REDIS_COMMAND_H__

#include <string>
#include <vector>

class redis_client;
class redis_reply;

class redis_command
{
public:
    redis_command(redis_client* client);
    virtual ~redis_command(){}

    void set_client(redis_client* client);

protected:
    void build_request(const std::vector<std::string>& argv);
    void hash_slots(std::string key);
    redis_reply* run();

/******************************************************************************/
    bool check_status(const char* expection = "OK");
    int check_status_or_nil(const char* expection = "OK");

    bool get_string(std::string& result);
    bool get_string(std::string* result);
    int get_string_or_nil(std::string& result);

    long long get_integer64(bool* success = NULL);
    bool get_integer64(long long& result);
    bool get_integer64(long long* result);
    int get_integer32(bool* success = NULL);
    long long get_integer64_or_nil(bool * success = NULL);
    int get_integer32_or_nil(bool * success = NULL);

    int get_array(std::vector<std::string*>& result);
    int get_array(std::vector<std::string>& result);
    int get_array_or_nil(std::vector<std::string>& result);
    int get_cursor_array(std::vector<std::string>* result);

/******************************************************************************/
    void scan_keys(const char* cmd, const std::string* key, int cursor,
                   const char * pattern, int count);

private:
    // 合成command命令, 方便日志输出
    void generate_cmdstr(const std::vector<std::string>& argv);
    // 分析redis_reply对象, 方便日志输出
    std::string parse_reply(const redis_reply* reply);

private:
    redis_client* m_client;
    std::string m_command;
    std::string m_request_buf;
};

#endif /* __REDIS_COMMAND_H__ */
