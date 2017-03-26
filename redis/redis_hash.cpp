#include "redis_client.h"
#include "redis_hash.h"
#include "redis_log.h"
#include "redis_helper.h"

long long redis_hash::hdel(const std::string& key,
                           const std::vector<std::string>& fields)
{
    std::string field_list = redis_helper::join(fields);
    build_command("HDEL %s %s", key.c_str(), field_list.c_str());
    hash_slots(key);

    return get_integer64();
}

int redis_hash::hexists(const std::string& key, const std::string& field)
{
    build_command("HEXISTS %s %s", key.c_str(), field.c_str());
    hash_slots(key);

    return get_integer32();
}

int redis_hash::hget(const std::string& key, const std::string& field,
                     std::string& result)
{
    build_command("HGET %s %s", key.c_str(), field.c_str());
    hash_slots(key);

    return get_string_or_nil(result);
}

bool redis_hash::hgetall(const std::string key,
                         std::map<std::string, std::string>& result)
{
    build_command("HGETALL %s", key.c_str());
    hash_slots(key);

    std::vector<std::string> key_val;
    bool bret = get_array(&key_val);

    ASSERT(key_val.size() % 2 == 0);
    for (size_t i = 0; i < key_val.size(); i += 2)
    {
        result[key_val[i]] = key_val[i + 1];
    }

    return bret;
}

bool redis_hash::hincrby(const std::string& key, const std::string& field,
                         long long increment, long long* result /*= NULL*/)
{
    build_command("HINCRBY %s %s %lld", key.c_str(), field.c_str(), increment);
    hash_slots(key);

    return get_integer64(result);
}

bool redis_hash::hincrbyfloat(const std::string key,
                              const std::string field,
                              double increment,
                              std::string* result /*= NULL*/)
{
    build_command("HINCRBYFLOAT %s %s %lf",
                  key.c_str(), field.c_str(), increment);
    hash_slots(key);

    return get_string(result);
}

bool redis_hash::hkeys(std::string key, std::vector<std::string>& result)
{
    build_command("HKEYS %s", key.c_str());
    hash_slots(key);

    return get_array(&result);
}

long long redis_hash::hlen(std::string key)
{
    build_command("HLEN %s", key.c_str());
    hash_slots(key);

    return get_integer64();
}

bool redis_hash::hmget(std::string key, std::vector<std::string>& fields, std::vector<std::string>& result)
{
    std::string field_list = redis_helper::join(fields);
    build_command("HMGET %s %s", key.c_str(), field_list.c_str());
    hash_slots(key);

    return get_array(&result);
}

bool redis_hash::hmset(std::string key, std::map<std::string, std::string>& field_values)
{
    std::string key_val = redis_helper::join(field_values);
    build_command("HMSET %s %s", key.c_str(), key_val.c_str());
    hash_slots(key);

    return check_status();
}

bool redis_hash::hscan(std::string key, int& cursor, std::map<std::string,std::string>& result)
{
    build_command("HSCAN %s %d", key.c_str(), cursor);
    hash_slots(key);

    std::vector<std::string> key_val;
    bool bret = get_cursor_array(cursor, &key_val);

    ASSERT(key_val.size() % 2 == 0);
    for (size_t i = 0; i < key_val.size(); i += 2)
    {
        result[key_val[i]] = key_val[i + 1];
    }

    return bret;
}

bool redis_hash::hset(std::string key, std::string field, std::string value)
{
    build_command("HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
    hash_slots(key);

    return get_integer64() ? true : false;
}

bool redis_hash::hsetnx(std::string key, std::string field, std::string value)
{
    build_command("HSETNX %s %s %s", key.c_str(), field.c_str(), value.c_str());
    hash_slots(key);

    return get_integer64() ? true : false;
}

long long redis_hash::hstrlen(std::string key, std::string field)
{
    build_command("HSTRLEN %s %s", key.c_str(), field.c_str());
    hash_slots(key);

    return get_integer64();
}

bool redis_hash::hvals(std::string key, std::vector<std::string>& result)
{
    build_command("HVALS %s", key.c_str());
    hash_slots(key);

    return get_array(&result);
}

