#include <iostream>

#include "redis_client.h"
#include "redis_string.h"
#include "redis_log.h"


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

long long redis_string::setbit(std::string key,int offset,int value)
{
    build_command("SETBIT %s %d %d", key.c_str(), offset, value);
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


