#ifndef __REDIS_REPLY_H__
#define __REDIS_REPLY_H__

#include <vector>
#include <string>

typedef enum {
    T_REDIS_REPLY_UNKOWN,     // 0
    T_REDIS_REPLY_STRING,     // 1
    T_REDIS_REPLY_ARRAY,      // 2
    T_REDIS_REPLY_INTEGER,    // 3
    T_REDIS_REPLY_NIL,        // 4
    T_REDIS_REPLY_STATUS,     // 5
    T_REDIS_REPLY_ERROR,      // 6
} t_redis_reply;

static const char * const REPLY_TYPE[] =
{
    "REDIS_REPLY_UNKOWN",   // 0
    "REDIS_REPLY_STRING",   // 1
    "REDIS_REPLY_ARRAY",    // 2
    "REDIS_REPLY_INTEGER",  // 3
    "REDIS_REPLY_NIL",      // 4
    "REDIS_REPLY_STATUS",   // 5
    "REDIS_REPLY_ERROR"     // 6
};

/**
 * redis-server返回结果对象类, 对redis-server返回的数据进行分析后创建redis_reply
 * 类对象。
 */
class redis_reply
{
public:
    redis_reply();
    ~redis_reply();

public:
    t_redis_reply get_type() const { return m_type; }
    int get_size() const;
    const redis_reply* get_element(size_t idx) const;
    std::string get_status() const;
    std::string get_error() const;
    std::string get_string() const { return m_str; }
    long long get_integer() const { return m_integer; }
    double get_double(bool* success = NULL) const;

public:
    redis_reply& set_type(t_redis_reply type);
    redis_reply& put(const std::string& buff);
    redis_reply& put(const redis_reply* rr);

private:
    void clear();

private:
    // REDIS_REPLY_*
    t_redis_reply m_type;
    // The integer when type is REDIS_REPLY_INTEGER
    long long m_integer;
    // Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING
    std::string m_str;
    // elements vector for REDIS_REPLY_ARRAY
    std::vector<const redis_reply*> m_element;
};

#endif /* __REDIS_REPLY_H__ */
