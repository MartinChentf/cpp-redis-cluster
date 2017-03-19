#include "redis_client.h"
#include "redis_hash.h"
#include "redis_log.h"

long long redis_hash::hdel(std::string key, std::vector<std::string>& fields)
{
    std::string field_list("");
    for (size_t i = 0; i < fields.size(); i ++) {
        field_list += fields[i] + " ";
    }

    build_command("HDEL %s %s", key.c_str(), field_list.c_str());
    hash_slots(key);

    return get_integer64();
}

bool redis_hash::hexists(std::string key, std::string field)
{
    build_command("HEXISTS %s %s", key.c_str(), field.c_str());
    hash_slots(key);

    return get_integer64() ? true : false;
}

std::string redis_hash::hget(std::string key, std::string field)
{
    build_command("HGET %s %s", key.c_str(), field.c_str());
    hash_slots(key);

    return get_string_or_nil();
}

bool redis_hash::hgetall(std::string key, std::map<std::string, std::string>& result)
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

long long redis_hash::hincrby(std::string key, std::string field, long long increment)
{
    build_command("HINCRBY %s %s %lld", key.c_str(), field.c_str(), increment);
    hash_slots(key);

    return get_integer64();
}

std::string redis_hash::hincrbyfloat(std::string key, std::string field, double increment)
{
    build_command("HINCRBYFLOAT %s %s %lf", key.c_str(), field.c_str(), increment);
    hash_slots(key);

    return get_string();
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
    std::string field_list("");
    for (size_t i = 0; i < fields.size(); i ++) {
        field_list += fields[i] + " ";
    }

    build_command("HMGET %s %s", key.c_str(), field_list.c_str());
    hash_slots(key);

    return get_array(&result);
}

bool redis_hash::hmset(std::string key, std::map<std::string, std::string>& field_values)
{
    std::string key_val;
    for (t_field_value_map_iter it = field_values.begin();
         it != field_values.end(); ++ it)
    {
        key_val += it->first + " " + it->second;
    }
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

