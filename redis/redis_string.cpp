#include "redis_string.h"
#include "redis_helper.h"
#include "redis_log.h"

const char* redis_string::BITOP_STR[redis_string::SIZE_BITOP]
    = { "AND", "OR", "NOT", "XOR" };

int redis_string::get(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("GET");
    argv.push_back(key.c_str());

    build_request(argv);
    hash_slots(key);    

    return get_string_or_nil(result);
}

int redis_string::getSet(const std::string& key,
                          const std::string& value,
                          std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("GETSET");
    argv.push_back(key.c_str());
    argv.push_back(value.c_str());

    build_request(argv);
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
    std::vector<std::string> argv;
    argv.push_back("SET");
    argv.push_back(key.c_str());
    argv.push_back(value.c_str());

    if (ex_px != NULL) {
        argv.push_back(ex_px);
        argv.push_back(TO_STRING(second));
    }
    if (nx_xx != NULL) {
        argv.push_back(nx_xx);
    }
    build_request(argv);
    hash_slots(key);

    return (check_status_or_nil() > 0 ? true : false);
}

bool redis_string::getrange(const std::string& key,int start,int end,
                            std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("GETRANGE");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(end));

    build_request(argv);
    hash_slots(key);

    return get_string(result);
}

bool redis_string::setrange(const std::string& key, int offset,
                            const std::string& value,
                            long long* length /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("SETRANGE");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(offset));
    argv.push_back(value.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer64(length);
}

int redis_string::getbit(const std::string& key,int offset)
{
    std::vector<std::string> argv;
    argv.push_back("GETBIT");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(offset));

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_string::setbit(const std::string& key, int offset, bool value)
{
    std::vector<std::string> argv;
    argv.push_back("SETBIT");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(offset));
    argv.push_back(TO_STRING(value?1:0));

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

long long redis_string::bitconut(const std::string& key)
{
    return bitconut(key, 0, -1);
}

long long redis_string::bitconut(const std::string& key, int start, int end)
{
    std::vector<std::string> argv;
    argv.push_back("BITCOUNT");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(end));

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

long long redis_string::bitop(BITOP op, const std::string& dest_key,
                              const std::vector<std::string>& src_keys)
{
    std::vector<std::string> argv;
    argv.push_back("BITOP");
    argv.push_back(BITOP_STR[op]);
    argv.push_back(dest_key.c_str());
    for (size_t i = 0; i < src_keys.size(); i++) {
        argv.push_back(src_keys[i]);
    }

    build_request(argv);
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
    std::vector<std::string> argv;
    argv.push_back("BITPOS");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(value?1:0));
    argv.push_back(TO_STRING(start));
    argv.push_back(TO_STRING(end));

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

bool redis_string::mget(const std::vector<std::string>& keys,
                        std::vector<std::string*>& result)
{
    std::vector<std::string> argv;
    argv.push_back("MGET");
    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }

    build_request(argv);
    if (!keys.empty()) {
        hash_slots(keys[0]);
    }

    return get_array(result) >= 0;
}

bool redis_string::mset(const std::map<std::string, std::string>& key_values)
{
    std::vector<std::string> argv;
    argv.push_back("MSET");
    std::map<std::string, std::string>::const_iterator cit = key_values.begin();
    while (cit != key_values.end()) {
        argv.push_back(cit->first);
        argv.push_back(cit->second);
        ++cit;
    }
    build_request(argv);
    return check_status("OK");
}

bool redis_string::msetnx(const std::map<std::string, std::string>& key_values)
{
    std::vector<std::string> argv;
    argv.push_back("MSETNX");

    std::map<std::string, std::string>::const_iterator it = key_values.begin();
    for (; it != key_values.end(); ++it) {
        argv.push_back(it->first);
        argv.push_back(it->second);
    }

    build_request(argv);
    if (!key_values.empty()) {
        hash_slots(key_values.begin()->first);
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
    std::vector<std::string> argv;
    argv.push_back("INCRBYFLOAT");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(increment));

    build_request(argv);
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
    std::vector<std::string> argv;
    argv.push_back(cmd);
    argv.push_back(key.c_str());

    if (inc != NULL) {
        argv.push_back(TO_STRING(*inc));
    }

    build_request(argv);
    hash_slots(key);

    return get_integer64(result);
}

bool redis_string::append(const std::string & key,const std::string & value,
                          long long * length /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("APPEND");
    argv.push_back(key.c_str());
    argv.push_back(value.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer64(length);
}

bool redis_string::strlen(const std::string & key,long long & length)
{
    std::vector<std::string> argv;
    argv.push_back("STRLEN");
    argv.push_back(key.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer64(length);
}

