#ifndef __REDIS_COMMAND_H__
#define __REDIS_COMMAND_H__

#include <stdint.h>
#include <string>
#include <vector>

#include "Connection.h"

class redisReply;

class redis_command : public Connection
{
public:
    redis_command(const std::string& host, uint16_t port);
    virtual ~redis_command(){}

/******************************************************************************/
    bool check_status(const char* expection = "OK");
    int check_status_or_nil(const char* expection = "OK");
    std::string get_status();

    std::string get_string(bool* success = NULL);
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

protected:
    // 覆盖基类中的 sendCommand 函数
    bool sendCommand(const std::vector<std::string>& argv);

private:
    // 合成command命令, 方便日志输出
    void generate_cmdstr(const std::vector<std::string>& argv);
    // 分析 redisReply 对象, 方便日志输出
    std::string parse_reply(const redisReply* reply);

private:
    std::string m_command;
};

#endif /* __REDIS_COMMAND_H__ */
