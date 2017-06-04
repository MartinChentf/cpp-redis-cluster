#include "redis_set.h"
#include "redis_helper.h"

redis_set::redis_set(const std::string & host, uint16_t port)
: redis_command(host, port)
{
}

long long redis_set::sadd(const std::string& key,
                          const std::vector<std::string>& member)
{
    std::vector<std::string> argv;
    argv.push_back("SADD");
    argv.push_back(key.c_str());
    for (size_t i = 0; i < member.size(); i++) {
        argv.push_back(member[i]);
    }

    sendCommand(argv);

    return get_integer64();
}

long long redis_set::scard(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("SCARD");
    argv.push_back(key.c_str());

    sendCommand(argv);

    return get_integer64();
}

bool redis_set::sdiff(const std::vector<std::string>& keys,
                      std::vector<std::string>& result)
{
    return set_operation("SDIFF", keys, result);
}

bool redis_set::sinter(const std::vector<std::string>& keys,
                       std::vector<std::string>& result)
{
    return set_operation("SINTER", keys, result);
}

bool redis_set::sunion(const std::vector<std::string>& keys,
                       std::vector<std::string>& result)
{
    return set_operation("SUNION", keys, result);
}

bool redis_set::set_operation(const char* op,
                              const std::vector<std::string>& keys,
                              std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back(op);
    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }

    sendCommand(argv);

    return get_array_or_nil(result) >= 0;
}

long long redis_set::sdiffstore(const std::string& dest,
                                const std::vector<std::string>& keys)
{
    return set_operation_with_store("SDIFFSTORE", dest, keys);
}

long long redis_set::sinterstore(const std::string& dest,
                                 const std::vector<std::string>& keys)
{
    return set_operation_with_store("SINTERSTORE", dest, keys);
}

long long redis_set::sunionstore(const std::string& dest,
                                 const std::vector<std::string>& keys)
{
    return set_operation_with_store("SUNIONSTORE", dest, keys);
}

long long 
redis_set::set_operation_with_store(const char* op,
                                    const std::string& dest,
                                    const std::vector<std::string>& keys)
{
    std::vector<std::string> argv;
    argv.push_back(op);
    argv.push_back(dest.c_str());
    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }

    sendCommand(argv);

    return get_integer64();
}

int redis_set::sismember(const std::string& key, const std::string& member)
{
    std::vector<std::string> argv;
    argv.push_back("SISMEMBER");
    argv.push_back(key.c_str());
    argv.push_back(member.c_str());

    sendCommand(argv);

    return get_integer32();
}

int redis_set::smembers(const std::string& key,
                        std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("SMEMBERS");
    argv.push_back(key.c_str());

    sendCommand(argv);

    return get_array_or_nil(result);
}

int redis_set::smove(const std::string& src, const std::string& dest,
                     const std::string& member)
{
    std::vector<std::string> argv;
    argv.push_back("SMOVE");
    argv.push_back(src.c_str());
    argv.push_back(dest.c_str());
    argv.push_back(member.c_str());

    sendCommand(argv);

    return get_integer32();
}

int redis_set::spop(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("SPOP");
    argv.push_back(key.c_str());

    sendCommand(argv);

    return get_string_or_nil(result);
}

int redis_set::spop(const std::string& key,
                    std::vector<std::string>& result, int count)
{
    std::vector<std::string> argv;
    argv.push_back("SPOP");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(count));

    sendCommand(argv);

    return get_array_or_nil(result);
}

int redis_set::srandmember(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("SRANDMEMBER");
    argv.push_back(key.c_str());

    sendCommand(argv);

    return get_string_or_nil(result);
}

int redis_set::srandmember(const std::string& key,
                           std::vector<std::string>& result, int count)
{
    std::vector<std::string> argv;
    argv.push_back("SRANDMEMBER");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(count));

    sendCommand(argv);

    return get_array_or_nil(result);
}

long long redis_set::srem(const std::string& key,
                          const std::vector<std::string>& member)
{
    std::vector<std::string> argv;
    argv.push_back("SREM");
    argv.push_back(key.c_str());
    for (size_t i = 0; i < member.size(); i++) {
        argv.push_back(member[i]);
    }

    sendCommand(argv);

    return get_integer64();
}

int redis_set::sscan(const std::string& key, int cursor,
                     std::vector<std::string>& result,
                     const char* pattern /*= NULL*/,
                     int count /*= 10*/)
{
    scan_keys("SSCAN", &key, cursor, pattern, count);
    return get_cursor_array(&result);
}

