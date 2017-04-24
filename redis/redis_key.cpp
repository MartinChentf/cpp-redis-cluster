#include "redis_key.h"
#include "redis_helper.h"

int redis_key::del(const std::vector<std::string>& keys)
{
    std::vector<std::string> argv;
    argv.push_back("DEL");

    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }

    build_request(argv);

    if (!keys.empty()) {
        hash_slots(keys[0]);
    }

    return get_integer32();
}

int redis_key::del(const std::string& key)
{
    std::vector<std::string> str;
    str.push_back(key);

    return del(str);
}

int redis_key::dump(const std::string& key, std::string& result)
{
    std::vector<std::string> argv;
    argv.push_back("DUMP");
    argv.push_back(key);

    build_request(argv);
    hash_slots(key);

    return get_string_or_nil(result);
}

int redis_key::exists(const std::vector<std::string>& keys)
{
    std::vector<std::string> argv;
    argv.push_back("EXISTS");

    for (size_t i = 0; i < keys.size(); i++) {
        argv.push_back(keys[i]);
    }

    build_request(argv);
    if (!keys.empty()) {
        hash_slots(keys[0]);
    }

    return get_integer32();
}

int redis_key::exists(const std::string& key)
{
    std::vector<std::string> argv;
    argv.push_back("EXISTS");
    argv.push_back(key);

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_key::expire(const std::string& key, int second)
{
    std::vector<std::string> argv;
    argv.push_back("EXPIRE");
    argv.push_back(key);
    argv.push_back(TO_STRING(second));

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_key::expireat(const std::string& key, time_t timestamp)
{
    std::vector<std::string> argv;
    argv.push_back("EXPIREAT");
    argv.push_back(key);
    argv.push_back(TO_STRING(timestamp));

    build_request(argv);
    hash_slots(key);

    return get_integer32();
}

int redis_key::keys(const std::string& pattern,std::vector<std::string>& result)
{
    std::vector<std::string> argv;
    argv.push_back("KEYS");
    argv.push_back(pattern);

    build_request(argv);

    return get_array_or_nil(result);
}

int redis_key::scan(int cursor, std::vector<std::string>& result,
                    const char * pattern /*= NULL*/, int count /*= 10*/)
{
    scan_keys("SCAN", NULL, cursor, pattern, count);
    return get_cursor_array(&result);
}

