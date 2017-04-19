#include "redis_list.h"
#include "redis_helper.h"

int redis_list::blpop(const std::vector<std::string>& keys, long long timeout,
                      std::map<std::string,std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("BLPOP");
    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }
    argv.push_back(TO_STRING(timeout));

    build_request(argv);
    if (keys.size()) {
        hash_slots(keys[0]);
    }

    std::vector<std::string> vec;
    int iret = get_array_or_nil(vec);

    if (iret == 2) { // 如果等于2, 返回值为1
        result[vec[0]] = vec[1];
        iret = 1;
    }
    else if (iret > 0) { // 如果是大于0的其他值, 返回值为-1
        iret = -1;
    }

    return iret;
}

int redis_list::brpop(const std::vector<std::string>& keys, long long timeout,
                      std::map<std::string,std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("BRPOP");
    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }
    argv.push_back(TO_STRING(timeout));

    build_request(argv);
    if (keys.size()) {
        hash_slots(keys[0]);
    }

    std::vector<std::string> vec;
    int iret = get_array_or_nil(vec);

    if (iret == 2) { // 如果等于2, 返回值为1
        result[vec[0]] = vec[1];
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

    build_request(argv);
    hash_slots(src);

    return get_string_or_nil(result);
}

int redis_list::lindex(const std::string& key, int index, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("LINDEX");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(index));

    build_request(argv);
    hash_slots(key);

    return get_string_or_nil(result);
}

long long redis_list::linsert(const std::string& key, bool is_before,
                            const std::string& pivot, const std::string& value)
{
    build_command("LINSERT %s %s %s %s", key.c_str(),
                  is_before ? "BEFORE" : "AFTER",
                  pivot.c_str(), value.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_list::llen(const std::string& key)
{
    build_command("LLEN %s", key.c_str());
    hash_slots(key);

    return get_integer64();
}

int redis_list::lpop(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("LPOP");
    argv.push_back(key.c_str());

    build_request(argv);
    hash_slots(key);

    return get_string_or_nil(result);
}

long long redis_list::lpush(const std::string& key,
                            const std::vector<std::string>& values)
{
    std::string value_list = redis_helper::join(values);
    build_command("LPUSH %s %s", key.c_str(), value_list.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_list::lpushx(const std::string& key, const std::string& value)
{
    build_command("LPUSHX %s %s", key.c_str(), value.c_str());
    hash_slots(key);

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

    build_request(argv);
    hash_slots(key);

    return get_array_or_nil(result) >= 0;
}

long long redis_list::lrem(const std::string& key, int count,
                           const std::string& value)
{
    build_command("LREM %s %d %s", key.c_str(), count, value.c_str());
    hash_slots(key);

    return get_integer64();
}

bool redis_list::lset(const std::string& key, int index,
                      const std::string& value)
{
#if 0
#define 0
    build_command("LSET %s %d %s", key.c_str(), index, value.c_str());
    hash_slots(key);
#endif /* 0 */

    std::vector<std::string> argv;
    argv.push_back("LSET");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(index));
    argv.push_back(value.c_str());
    build_request(argv);
    hash_slots(key);

    return check_status();
}

bool redis_list::ltrim(const std::string& key, int start, int stop)
{
#if 0
    build_command("LTRIM %s %d %d", key.c_str(), start, stop);
    hash_slots(key);
#endif /* 0 */

    std::vector<std::string> argv;
    argv.push_back("LTRIM");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(stop));
    build_request(argv);
    hash_slots(key);

    return check_status();
}

int redis_list::rpop(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("RPOP");
    argv.push_back(key.c_str());

    build_request(argv);
    hash_slots(key);

    return get_string_or_nil(result);
}

int redis_list::rpoplpush(const std::string& src, const std::string& dest,
                          std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("RPOPLPUSH");
    argv.push_back(src.c_str());
    argv.push_back(dest.c_str());

    build_request(argv);
    hash_slots(src);

    return get_string_or_nil(result);
}

long long redis_list::rpush(const std::string& key,
                            const std::vector<std::string>& values)
{
    std::string value_list = redis_helper::join(values);
    build_command("RPUSH %s %s", key.c_str(), value_list.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_list::rpushx(const std::string& key, const std::string& value)
{
    build_command("RPUSHX %s %s", key.c_str(), value.c_str());
    hash_slots(key);

    return get_integer64();
}

