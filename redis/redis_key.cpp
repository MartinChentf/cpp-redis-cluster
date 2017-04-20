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

int redis_key::scan(int cursor, std::vector<std::string>& result,
                    const char * pattern /*= NULL*/, int count /*= 10*/)
{
    scan_keys("SCAN", NULL, cursor, pattern, count);
    return get_cursor_array(&result);
}

