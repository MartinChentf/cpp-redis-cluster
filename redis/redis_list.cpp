#include "redis_list.h"
#include "redis_helper.h"

redis_list::redis_list(const std::string & host, uint16_t port)
: redis_command(host, port)
{
}

int redis_list::blpop(const std::vector<std::string>& keys, long long timeout,
                      std::string& key, std::string& value)
{
    std::vector<std::string> argv;
    argv.push_back("BLPOP");
    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }
    argv.push_back(TO_STRING(timeout));

    sendCommand(argv);

    std::vector<std::string> vec;
    int iret = get_array_or_nil(vec);

    if (iret == 2) { // 如果等于2, 返回值为1
        key = vec[0];
        value = vec[1];
        iret = 1;
    }
    else if (iret > 0) { // 如果是大于0的其他值, 返回值为-1
        iret = -1;
    }

    return iret;
}

int redis_list::brpop(const std::vector<std::string>& keys, long long timeout,
                      std::string& key, std::string& value)
{
    std::vector<std::string> argv;
    argv.push_back("BRPOP");
    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }
    argv.push_back(TO_STRING(timeout));

    sendCommand(argv);

    std::vector<std::string> vec;
    int iret = get_array_or_nil(vec);

    if (iret == 2) { // 如果等于2, 返回值为1
        key = vec[0];
        value = vec[1];
        iret = 1;
    }
    else if (iret > 0) { // 如果是大于0的其他值, 返回值为-1
        iret = -1;
    }

    return iret;
}

int redis_list::brpoplpush(const std::string& src, const std::string& dest,
                           long long timeout, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("BRPOPLPUSH");
    argv.push_back(src.c_str());
    argv.push_back(dest.c_str());
    argv.push_back(TO_STRING(timeout));

    sendCommand(argv);

    return get_string_or_nil(result);
}

int redis_list::lindex(const std::string& key, int index, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("LINDEX");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(index));

    sendCommand(argv);

    return get_string_or_nil(result);
}

long long redis_list::linsert(const std::string& key, bool is_before,
                            const std::string& pivot, const std::string& value)
{
    std::vector<std::string> argv;
    argv.push_back("LINSERT");
    argv.push_back(key.c_str());
    argv.push_back(is_before ? "BEFORE" : "AFTER");
    argv.push_back(pivot.c_str());
    argv.push_back(value.c_str());

    sendCommand(argv);

    return get_integer64();
}

long long redis_list::llen(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("LLEN");
    argv.push_back(key.c_str());

    sendCommand(argv);

    return get_integer64();
}

int redis_list::lpop(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("LPOP");
    argv.push_back(key.c_str());

    sendCommand(argv);

    return get_string_or_nil(result);
}

long long redis_list::lpush(const std::string& key,
                            const std::vector<std::string>& values)
{
    std::vector<std::string> argv;
    argv.push_back("LPUSH");
    argv.push_back(key.c_str());
    for (size_t i = 0; i < values.size(); i++) {
        argv.push_back(values[i]);
    }

    sendCommand(argv);

    return get_integer64();
}

long long redis_list::lpushx(const std::string& key, const std::string& value)
{
    std::vector<std::string> argv;
    argv.push_back("LPUSHX");
    argv.push_back(key.c_str());
    argv.push_back(value.c_str());

    sendCommand(argv);

    return get_integer64();
}

bool redis_list::lrange(const std::string& key, int start, int stop,
                        std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("LRANGE");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(stop));

    sendCommand(argv);

    return get_array_or_nil(result) >= 0;
}

long long redis_list::lrem(const std::string& key, int count,
                           const std::string& value)
{
    std::vector<std::string> argv;
    argv.push_back("LREM");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(count));
    argv.push_back(value.c_str());

    sendCommand(argv);

    return get_integer64();
}

bool redis_list::lset(const std::string& key, int index,
                      const std::string& value)
{
    std::vector<std::string> argv;
    argv.push_back("LSET");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(index));
    argv.push_back(value.c_str());

    sendCommand(argv);

    return check_status();
}

bool redis_list::ltrim(const std::string& key, int start, int stop)
{
    std::vector<std::string> argv;
    argv.push_back("LTRIM");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(stop));

    sendCommand(argv);

    return check_status();
}

int redis_list::rpop(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("RPOP");
    argv.push_back(key.c_str());

    sendCommand(argv);

    return get_string_or_nil(result);
}

int redis_list::rpoplpush(const std::string& src, const std::string& dest,
                          std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("RPOPLPUSH");
    argv.push_back(src.c_str());
    argv.push_back(dest.c_str());

    sendCommand(argv);

    return get_string_or_nil(result);
}

long long redis_list::rpush(const std::string& key,
                            const std::vector<std::string>& values)
{
    std::vector<std::string> argv;
    argv.push_back("RPUSH");
    argv.push_back(key.c_str());
    for (size_t i = 0; i < values.size(); i++) {
        argv.push_back(values[i]);
    }

    sendCommand(argv);

    return get_integer64();
}

long long redis_list::rpushx(const std::string& key, const std::string& value)
{
    std::vector<std::string> argv;
    argv.push_back("RPUSHX");
    argv.push_back(key.c_str());
    argv.push_back(value.c_str());

    sendCommand(argv);

    return get_integer64();
}

