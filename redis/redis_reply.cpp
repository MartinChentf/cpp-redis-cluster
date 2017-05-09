#include <stdlib.h>

#include "redis_reply.h"
#include "redis_helper.h"
#include "redis_log.h"

redis_reply::redis_reply()
: m_type(REDIS_REPLY_UNKOWN)
, m_integer(0)
, m_str("")
{
    clear();
}

redis_reply::~redis_reply()
{
    clear();
}

void redis_reply::clear()
{
    for (size_t i = 0; i < m_element.size(); i++) {
        delete m_element[i];
        m_element[i] = NULL;
    }
}

size_t redis_reply::get_size() const
{
    switch (m_type) {
        case REDIS_REPLY_NIL:
            return 0;
        case REDIS_REPLY_STRING:
        case REDIS_REPLY_INTEGER:
        case REDIS_REPLY_STATUS:
        case REDIS_REPLY_ERROR:
            return 1;
        case REDIS_REPLY_ARRAY:
            return m_element.size();
        case REDIS_REPLY_UNKOWN:
        default:
            return -1;
    }
}

const redis_reply* redis_reply::get_element(size_t idx) const
{
    if (m_element.size() <= 0 || idx >= m_element.size()) {
        return NULL;
    }
    return m_element[idx];
}

std::string redis_reply::get_status() const
{
    if (m_type != REDIS_REPLY_STATUS) {
        return "";
    }
    return m_str;
}

std::string redis_reply::get_error() const
{
    if (m_type != REDIS_REPLY_ERROR) {
        return "";
    }
    return m_str;
}

double redis_reply::get_double(bool * success /*= NULL*/) const
{
    SAFE_ASSIGN(success, false);
    if (m_type != REDIS_REPLY_STRING) {
        return -1;
    }
    const char* ptr = m_str.c_str();
    if (*ptr == '\0') {
        return -1;
    }
    SAFE_ASSIGN(success, true);
    return atof(ptr);
}

redis_reply& redis_reply::set_type(t_redis_reply type)
{
    m_type = type;
    return *this;
}

redis_reply& redis_reply::put(const std::string & buff)
{
    switch (m_type) {
        case REDIS_REPLY_STRING:
        case REDIS_REPLY_STATUS:
        case REDIS_REPLY_ERROR:
            m_str = buff;
            break;
        case REDIS_REPLY_INTEGER:
            m_integer = atoll(buff.c_str());
            break;
        default:
            ERROR("unexpected type: %s", REPLY_TYPE[m_type]);
    }
    return *this;
}

redis_reply& redis_reply::put(const char * buff, int len)
{
    std::string str;
    for (int i = 0; i < len; i ++) {
        str.push_back(buff[i]);
    }
    return put(str);
}

redis_reply& redis_reply::put(const redis_reply * rr)
{
    if (m_type != REDIS_REPLY_ARRAY) {
        WARN("type(%s) isn't REDIS_REPLY_ARRAY", REPLY_TYPE[m_type]);
        return *this;
    }

    m_element.push_back(rr);

    return *this;
}

