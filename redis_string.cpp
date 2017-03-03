#include <iostream>

#include "redis_client.h"
#include "redis_string.h"
#include "redis_log.h"


const char* redis_string::BITOP_STR[SIZE_BITOP] = { "AND", "OR", "NOT", "XOR" };

bool redis_string::set(std::string key, std::string value)
{
    build_command("SET %s %s", key.c_str(), value.c_str());
    hash_slots(key);

    return check_status();
}

std::string redis_string::get(std::string key)
{
    build_command("GET %s", key.c_str());
    hash_slots(key);    

    return get_string_or_nil();
}

bool redis_string::setnx(std::string key, std::string value)
{
    build_command("SET %s %s NX", key.c_str(), value.c_str());
    hash_slots(key);

    return check_status_or_nil();
}

bool redis_string::setex(std::string key, long long second, std::string value)
{
    build_command("SET %s %s EX %d", key.c_str(), value.c_str(), second);
    hash_slots(key);

    return check_status();
}

bool redis_string::psetex(std::string key, long long millisecond, std::string value)
{
    build_command("SET %s %s PX %d", key.c_str(), value.c_str(), millisecond);
    hash_slots(key);

    return check_status();
}

std::string redis_string::getrange(std::string key,int start,int end)
{
    build_command("GETRANGE %s %d %d", key.c_str(), start, end);
    hash_slots(key);

    return get_string();
}

long long redis_string::setrange(std::string key,int offset, std::string value)
{
    build_command("SETRANGE %s %d %s", key.c_str(), offset, value.c_str());
    hash_slots(key);

    return get_integer64();
}

std::string redis_string::getSet(std::string key, std::string value)
{
    build_command("GETSET %s %s", key.c_str(), value.c_str());
    hash_slots(key);

    return get_string_or_nil();
}

long long redis_string::getbit(std::string key,int offset)
{
    build_command("GETBIT %s %d", key.c_str(), offset);
    hash_slots(key);

    return get_integer64();
}

long long redis_string::setbit(std::string key, int offset, bool value)
{
    build_command("SETBIT %s %d %d", key.c_str(), offset, value);
    hash_slots(key);

    return get_integer64();
}

long long redis_string::bitconut(std::string key)
{
    return bitconut(key, 0, -1);
}

long long redis_string::bitconut(std::string key, int start, int end)
{
    build_command("BITCOUNT %s %d %d", key.c_str(), start, end);
    hash_slots(key);

    return get_integer64();
}

long long redis_string::bitop(BITOP op, std::string dest_key, std::vector<std::string>& src_keys)
{
    std::string key_list;
    for (size_t i = 0; i < src_keys.size(); i++) {
        key_list += src_keys[i] + " ";
    }

    build_command("BITOP %s %s %s", BITOP_STR[op], dest_key.c_str(), key_list.c_str());
    hash_slots(dest_key);

    return get_integer64();
}

long long redis_string::bitpos(std::string key,bool value,int start /*= 0*/,int end /*= -1*/)
{
    build_command("BITPOS %s %d %d %d", key.c_str(), value, start, end);
    hash_slots(key);

    return get_integer64();
}

bool redis_string::mget(std::vector<std::string>& keys, std::vector<std::string>& result)
{
    return mget(keys, &result);
}

bool redis_string::mget(std::vector<std::string>& keys, std::vector<std::string>* result)
{
    std::string key_list;
    for (size_t i = 0; i < keys.size(); i++) {
        key_list += keys[i] + " ";
    }

    build_command("MGET %s", key_list.c_str());
    if (!keys.empty()) {
        hash_slots(keys[0]);
    }

    return get_array(result);
}

bool redis_string::mset(std::map<std::string, std::string>& keyValues)
{
    std::string key_value_list;
    std::map<std::string, std::string>::iterator it = keyValues.begin();
    for (; it != keyValues.end(); ++it) {
        key_value_list += it->first + " " + it->second + " ";
    }

    build_command("MSET %s", key_value_list.c_str());
    if (!key_value_list.empty()) {
        hash_slots(keyValues.begin()->first);
    }

    return check_status();
}

bool redis_string::msetnx(std::map<std::string, std::string>& keyValues)
{
    std::string key_value_list;
    std::map<std::string, std::string>::iterator it = keyValues.begin();
    for (; it != keyValues.end(); ++it) {
        key_value_list += it->first + " " + it->second + " ";
    }

    build_command("MSETNX %s", key_value_list.c_str());
    if (!key_value_list.empty()) {
        hash_slots(keyValues.begin()->first);
    }

    return (get_integer64() == 1 ? true : false);
}

long long redis_string::incr(std::string key)
{
    build_command("INCR %s", key.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_string::incrBy(std::string key, int increment)
{
    build_command("INCRBY %s %d", key.c_str(), increment);
    hash_slots(key);

    return get_integer64();
}

std::string redis_string::incrByFloat(std::string key, double increment)
{
    build_command("INCRBYFLOAT %s %lf", key.c_str(), increment);
    hash_slots(key);

    return get_string();
}

long long redis_string::decr(std::string key)
{
    build_command("DECR %s", key.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_string::decrBy(std::string key, int decrement)
{
    build_command("DECRBY %s %d", key.c_str(), decrement);
    hash_slots(key);

    return get_integer64();
}

long long redis_string::append(std::string key, std::string value)
{
    build_command("APPEND %s %s", key.c_str(), value.c_str());
    hash_slots(key);

    return get_integer64();
}

long long redis_string::strlen(std::string key)
{
    build_command("STRLEN %s", key.c_str());
    hash_slots(key);

    return get_integer64();
}

