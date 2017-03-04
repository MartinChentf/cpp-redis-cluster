#ifndef __REDIS_HELPER_H__
#define __REDIS_HELPER_H__

#include <sstream>
#include <string>

class redis_helper
{
public:
    template<typename T>
    static std::string to_string(T a) {
        std::ostringstream ss;
        ss << a;
        return ss.str();
    }
};

static const char * REPLY_TYPE[] =
{
    "",
    "REDIS_REPLY_STRING",   // 1
    "REDIS_REPLY_ARRAY",    // 2
    "REDIS_REPLY_INTEGER",  // 3
    "REDIS_REPLY_NIL",      // 4
    "REDIS_REPLY_STATUS",   // 5
    "REDIS_REPLY_ERROR"     // 6
};

#endif /* __REDIS_HELPER_H__ */
