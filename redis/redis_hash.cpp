#include "redis_hash.h"
#include "redis_log.h"
#include "redis_helper.h"

long long redis_hash::hdel(const std::string& key,
                           const std::vector<std::string>& fields)
{
    std::vector<std::string> argv;
    argv.push_back("HDEL");
    argv.push_back(key.c_str());
    for (size_t i = 0; i < fields.size(); i++) {
        argv.push_back(fields[i]);
    }

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

int redis_hash::hexists(const std::string& key, const std::string& field)
{
    std::vector<std::string> argv;
    argv.push_back("HEXISTS");
    argv.push_back(key.c_str());
    argv.push_back(field.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_hash::hget(const std::string& key, const std::string& field,
                     std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("HGET");
    argv.push_back(key.c_str());
    argv.push_back(field.c_str());

    build_request(argv);
    hash_slots(key);

    return get_string_or_nil(result);
}

int redis_hash::hgetall(const std::string& key,
                         std::map<std::string, std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("HGETALL");
    argv.push_back(key.c_str());

    build_request(argv);
    hash_slots(key);

    std::vector<std::string> key_val;
    int iret = get_array_or_nil(key_val);

    if (iret > 0 && (iret % 2 == 0)) {  // iret为大于0的偶数
        for (int i = 0; i < iret; i += 2)
        {
            result[key_val[i]] = key_val[i + 1];
        }
        return iret / 2;
    }
    else if (iret > 0) { // iret为大于0的奇数
        return -1;
    }

    return iret;
}

bool redis_hash::hincrby(const std::string& key, const std::string& field,
                         long long increment, long long* result /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("HINCRBY");
    argv.push_back(key.c_str());
    argv.push_back(field.c_str());
    argv.push_back(TO_STRING(increment));

    build_request(argv);
    hash_slots(key);

    return get_integer64(result);
}

bool redis_hash::hincrbyfloat(const std::string& key,
                              const std::string& field,
                              double increment,
                              std::string* result /*= NULL*/)
{
    std::vector<std::string> argv;
    argv.push_back("HINCRBYFLOAT");
    argv.push_back(key.c_str());
    argv.push_back(field.c_str());
    argv.push_back(TO_STRING(increment));

    build_request(argv);
    hash_slots(key);

    return get_string(result);
}

bool redis_hash::hkeys(const std::string& key, std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("HKEYS");
    argv.push_back(key.c_str());

    build_request(argv);
    hash_slots(key);

    return get_array_or_nil(result) >= 0;
}

long long redis_hash::hlen(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("HLEN");
    argv.push_back(key.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

bool redis_hash::hmget(const std::string& key,
                       const std::vector<std::string>& fields,
                       std::vector<std::string*>& result)
{
    std::vector<std::string> argv;
    argv.push_back("HMGET");
    argv.push_back(key.c_str());
    for (size_t i = 0; i < fields.size(); i++) {
        argv.push_back(fields[i]);
    }

    build_request(argv);
    hash_slots(key);

    return get_array(result) >= 0;
}

bool redis_hash::hmset(const std::string& key,
                       const std::map<std::string, std::string>& field_values)
{
    std::vector<std::string> argv;
    argv.push_back("HMSET");
    argv.push_back(key.c_str());
    std::map<std::string, std::string>::const_iterator cit = field_values.begin();
    while (cit != field_values.end()) {
        argv.push_back(cit->first);
        argv.push_back(cit->second);
        ++cit;
    }

    build_request(argv);
    hash_slots(key);

    return check_status();
}

int redis_hash::hscan(const std::string& key, int cursor,
                      std::map<std::string,std::string>& result,
                      const char* pattern /*= NULL*/, int count /*= 10*/)
{
    std::vector<std::string> argv;
    argv.push_back("HSCAN");
    argv.push_back(key.c_str());
    argv.push_back(TO_STRING(cursor));
    if (pattern) {
        argv.push_back("MATCH");
        argv.push_back(pattern);
    }
    if (count != 10) {
        argv.push_back("COUNT");
        argv.push_back(TO_STRING(count));
    }

    build_request(argv);
    hash_slots(key);

    std::vector<std::string> key_val;
    cursor = get_cursor_array(&key_val);

    ASSERT(key_val.size() % 2 == 0);
    for (size_t i = 0; i < key_val.size(); i += 2)
    {
        result[key_val[i]] = key_val[i + 1];
    }

    return cursor;
}

int redis_hash::hset(const std::string& key, const std::string& field,
                     const std::string& value)
{
    std::vector<std::string> argv;
    argv.push_back("HSET");
    argv.push_back(key.c_str());
    argv.push_back(field.c_str());
    argv.push_back(value.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_hash::hsetnx(const std::string& key, const std::string& field,
                       const std::string& value)
{
    std::vector<std::string> argv;
    argv.push_back("HSETNX");
    argv.push_back(key.c_str());
    argv.push_back(field.c_str());
    argv.push_back(value.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

long long redis_hash::hstrlen(const std::string& key, const std::string& field)
{
    std::vector<std::string> argv;
    argv.push_back("HSTRLEN");
    argv.push_back(key.c_str());
    argv.push_back(field.c_str());

    build_request(argv);
    hash_slots(key);

    return get_integer64();
}

bool redis_hash::hvals(const std::string& key, std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("HVALS");
    argv.push_back(key.c_str());

    build_request(argv);
    hash_slots(key);

    return get_array_or_nil(result) >= 0;
}

