#include "redis_client.h"
#include "redis_list.h"
#include "redis_helper.h"

int redis_list::blpop(const std::vector<std::string>& keys, long long timeout,
                      std::map<std::string,std::string>& result)
{
    std::string key_list = redis_helper::join(keys);
    build_command("BLPOP %s %s %d", key_list.c_str(), timeout);
    if (keys.size()) {
        hash_slots(keys[0]);
    }

    std::vector<std::string> vec;
    int iret = get_array_or_nil(vec);

    if (vec.size() != 2) {
        iret = -1;
    }

    if (iret > 0) {
        result[vec[0]] = vec[1];
    }

    return iret;
}

int redis_list::brpop(const std::vector<std::string>& keys, long long timeout,
                      std::map<std::string,std::string>& result)
{
    std::string key_list = redis_helper::join(keys);
    build_command("BRPOP %s %s %d", key_list.c_str(), timeout);
    if (keys.size()) {
        hash_slots(keys[0]);
    }

    std::vector<std::string> vec;
    int iret = get_array_or_nil(vec);

    if (vec.size() != 2) {
        iret = -1;
    }

    if (iret > 0) {
        result[vec[0]] = vec[1];
    }

    return iret;
}

int redis_list::brpoplpush(const std::string& src, const std::string& dest,
                           long long timeout, std::string& result)
{
    build_command("BRPOPLPUSH %s %s %d", src.c_str(), dest.c_str(), timeout);
    hash_slots(src);

    return get_string_or_nil(result);
}

int redis_list::lindex(const std::string& key, int index, std::string& result)
{
    build_command("LINDEX %s %s", key.c_str(), index);
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
    build_command("LPOP %s", key.c_str());
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
    build_command("LRANGE %s %d %d", key.c_str(), start, stop);
    hash_slots(key);

    return get_array(result);
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
    build_command("LSET %s %d %s", key.c_str(), index, value.c_str());
    hash_slots(key);

    return check_status();
}

bool redis_list::ltrim(const std::string& key, int start, int stop)
{
    build_command("LTRIM %s %d %d", key.c_str(), start, stop);
    hash_slots(key);

    return check_status();
}

int redis_list::rpop(const std::string& key, std::string& result)
{
    build_command("RPOP %s", key.c_str());
    hash_slots(key);

    return get_string_or_nil(result);
}

int redis_list::rpoplpush(const std::string& src, const std::string& dest,
                          std::string& result)
{
    build_command("RPOPLPUSH %s %s", src.c_str(), dest.c_str());
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

