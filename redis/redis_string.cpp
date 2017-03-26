#include "redis_client.h"
#include "redis_string.h"
#include "redis_helper.h"


const char* redis_string::BITOP_STR[redis_string::SIZE_BITOP]
    = { "AND", "OR", "NOT", "XOR" };

int redis_string::get(const std::string& key, std::string& result)
{
    build_command("GET %s", key.c_str());
    hash_slots(key);    

    return get_string_or_nil(result);
}

int redis_string::getSet(const std::string& key,
                          const std::string& value,
                          std::string& result)
{
    build_command("GETSET %s %s", key.c_str(), value.c_str());
    hash_slots(key);

    return get_string_or_nil(result);
}

bool redis_string::set(const std::string& key, const std::string& value)
{
    return set_string(key, value, NULL, 0, NULL);
}

bool redis_string::setnx(const std::string& key, const std::string& value)
{
    return set_string(key, value, NULL, 0, "NX");
}

bool redis_string::setxx(const std::string& key, const std::string& value)
{
    return set_string(key, value, NULL, 0, "XX");
}

bool redis_string::setex(const std::string& key, long long second,
                         const std::string& value)
{
    return set_string(key, value, "EX", second, NULL);
}

bool redis_string::psetex(const std::string& key, long long millisecond,
                          const std::string& value)
{
    return set_string(key, value, "PX", millisecond, NULL);
}

bool redis_string::set_string(const std::string& key,const std::string& value,
                        const char * ex_px,long long second,const char * nx_xx)
{
    if (ex_px != NULL) {
        build_command("SET %s %s %s %lld %s",
                      key.c_str(), value.c_str(),
                      ex_px, second,
                      nx_xx == NULL ? "" : nx_xx);
    }
    else {
        build_command("SET %s %s %s", key.c_str(), value.c_str(),
                      nx_xx == NULL ? "" : nx_xx);
    }
    hash_slots(key);

    return (check_status_or_nil() > 0 ? true : false);
}

bool redis_string::getrange(const std::string& key,int start,int end,
                            std::string& result)
{
    build_command("GETRANGE %s %d %d", key.c_str(), start, end);
    hash_slots(key);

    return get_string(result);
}

bool redis_string::setrange(const std::string& key, int offset,
                            const std::string& value,
                            long long* length /*= NULL*/)
{
    build_command("SETRANGE %s %d %s", key.c_str(), offset, value.c_str());
    hash_slots(key);

    return get_integer64(length);
}

int redis_string::getbit(const std::string& key,int offset)
{
    build_command("GETBIT %s %d", key.c_str(), offset);
    hash_slots(key);

    return get_integer32();
}

int redis_string::setbit(const std::string& key, int offset, bool value)
{
    build_command("SETBIT %s %d %d", key.c_str(), offset, (value?1:0));
    hash_slots(key);

    return get_integer32();
}

long long redis_string::bitconut(const std::string& key)
{
    return bitconut(key, 0, -1);
}

long long redis_string::bitconut(const std::string& key, int start, int end)
{
    build_command("BITCOUNT %s %d %d", key.c_str(), start, end);
    hash_slots(key);

    return get_integer64();
}

long long redis_string::bitop(BITOP op, const std::string& dest_key,
                              const std::vector<std::string>& src_keys)
{
    std::string key_list = redis_helper::join(src_keys);
    build_command("BITOP %s %s %s", BITOP_STR[op],
                  dest_key.c_str(), key_list.c_str());
    hash_slots(dest_key);

    return get_integer64();
}

long long redis_string::bitop(BITOP op, const std::string& dest_key,
                              const std::string& src_keys)
{
    std::vector<std::string> str;
    str.push_back(src_keys);

    return bitop(op, dest_key, str);
}

long long redis_string::bitpos(const std::string& key,bool value,
                               int start /*= 0*/,int end /*= -1*/)
{
    build_command("BITPOS %s %d %d %d", key.c_str(), (value?1:0), start, end);
    hash_slots(key);

    return get_integer64();
}

bool redis_string::mget(const std::vector<std::string>& keys,
                        std::vector<std::string*>& result)
{
    return mget(keys, &result);
}

bool redis_string::mget(const std::vector<std::string>& keys,
                        std::vector<std::string*>* result)
{
    std::string key_list = redis_helper::join(keys);
    build_command("MGET %s", key_list.c_str());
    if (!keys.empty()) {
        hash_slots(keys[0]);
    }

    return get_array(result);
}

bool redis_string::mset(const std::map<std::string, std::string>& keyValues)
{
    std::string key_value_list = redis_helper::join(keyValues);
    build_command("MSET %s", key_value_list.c_str());
    if (!key_value_list.empty()) {
        hash_slots(keyValues.begin()->first);
    }

    return check_status();
}

bool redis_string::msetnx(const std::map<std::string, std::string>& keyValues)
{
    std::string key_value_list = redis_helper::join(keyValues);
    build_command("MSETNX %s", key_value_list.c_str());
    if (!key_value_list.empty()) {
        hash_slots(keyValues.begin()->first);
    }

    return (get_integer64() >= 1 ? true : false);
}

bool redis_string::incr(const std::string& key,long long * result /*= NULL*/)
{
    return incoper("INCR", key, NULL, result);
}

bool redis_string::incrby(const std::string& key,
                          long long increment,
                          long long * result /*= NULL*/)
{
    return incoper("INCRBY", key, &increment, result);
}

bool redis_string::incrbyfloat(const std::string & key,
                                      double increment,
                                      std::string * result /*= NULL*/)
{
    build_command("INCRBYFLOAT %s %lf", key.c_str(), increment);
    hash_slots(key);

    return get_string(result);
}

bool redis_string::decr(const std::string & key,long long * result /*= NULL*/)
{
    return incoper("DECR", key, NULL, result);
}

bool redis_string::decrby(const std::string & key,
                          long long decrement,
                          long long * result /*= NULL*/)
{
    return incoper("DECRBY", key, &decrement, result);
}

bool redis_string::incoper(const char * cmd,const std::string & key,
                           long long * inc,long long * result)
{
    if (inc != NULL)
        build_command("%s %s %d", cmd, key.c_str(), inc);
    else
        build_command("%s %s", cmd, key.c_str());

    hash_slots(key);

    return get_integer64(result);
}

bool redis_string::append(const std::string & key,const std::string & value,
                          long long * length /*= NULL*/)
{
    build_command("APPEND %s %s", key.c_str(), value.c_str());
    hash_slots(key);

    return get_integer64(length);
}

bool redis_string::strlen(const std::string & key,long long & length)
{
    build_command("STRLEN %s", key.c_str());
    hash_slots(key);

    return get_integer64(length);
}

