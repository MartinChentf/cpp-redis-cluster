#include "redis_client.h"
#include "redis_list.h"
#include "redis_helper.h"

std::string redis_list::lpop(std::string key)
{
    build_command("LPOP %s", key.c_str());
    hash_slots(key);

    return get_string_or_nil();
}

long long redis_list::lpush(std::string key, std::vector<std::string>& values)
{
    std::string value_list = redis_helper::join(values);
    build_command("LPUSH %s %s", key.c_str(), value_list.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_list::lpushx(std::string key,std::string value)
{
    build_command("LPUSHX %s %s", key.c_str(), value.c_str());
    hash_slots(key);

    return get_integer64();
}

std::string redis_list::rpop(std::string key)
{
    build_command("RPOP %s", key.c_str());
    hash_slots(key);

    return get_string_or_nil();
}

long long redis_list::rpush(std::string key, std::vector<std::string>& values)
{
    std::string value_list = redis_helper::join(values);
    build_command("RPUSH %s %s", key.c_str(), value_list.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_list::rpushx(std::string key,std::string value)
{
    build_command("RPUSHX %s %s", key.c_str(), value.c_str());
    hash_slots(key);

    return get_integer64();
}

std::string redis_list::rpoplpush(std::string src, std::string dest)
{
    build_command("RPOPLPUSH %s %s", src.c_str(), dest.c_str());
    hash_slots(src);

    return get_string_or_nil();
}

std::string redis_list::brpoplpush(std::string src, std::string dest, long long timeout)
{
    build_command("BRPOPLPUSH %s %s %d", src.c_str(), dest.c_str(), timeout);
    hash_slots(src);

    return get_string_or_nil();
}

std::string redis_list::lindex(std::string key, int index)
{
    build_command("LINDEX %s %s", key.c_str(), index);
    hash_slots(key);

    return get_string_or_nil();
}

long long redis_list::linsert(std::string key, bool is_before, std::string pivot, std::string value)
{
    build_command("LINSERT %s %s %s %s", key.c_str(),
                  is_before ? "BEFORE" : "AFTER",
                  pivot.c_str(), value.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_list::llen(std::string key)
{
    build_command("LLEN %s", key.c_str());
    hash_slots(key);

    return get_integer64();
}

bool redis_list::lrange(std::string key, int start, int stop, std::vector<std::string>& result)
{
    build_command("LRANGE %s %d %d", key.c_str(), start, stop);
    hash_slots(key);

    return get_array(&result);
}

long long redis_list::lrem(std::string key, int count, std::string value)
{
    build_command("LREM %s %d %s", key.c_str(), count, value.c_str());
    hash_slots(key);

    return get_integer64();
}

bool redis_list::lset(std::string key, int index, std::string value)
{
    build_command("LSET %s %d %s", key.c_str(), index, value.c_str());
    hash_slots(key);

    return check_status();
}

bool redis_list::ltrim(std::string key, int start, int stop)
{
    build_command("LTRIM %s %d %d", key.c_str(), start, stop);
    hash_slots(key);

    return check_status();
}

