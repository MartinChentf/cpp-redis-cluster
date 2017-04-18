#include <stdlib.h>

#include "redis_reply.h"

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
    for (int i = 0; i < m_element.size(); i++) {
        delete m_element[i];
        m_element[i] = NULL;
    }
}

redis_reply& redis_reply::set_type(t_redis_reply type)
{
    m_type = type;
    return *this;
}

redis_reply& redis_reply::put(const std::string& buff)
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

redis_reply& redis_reply::put(const redis_reply * rr)
{
    if (m_type != REDIS_REPLY_ARRAY) {
        WARN("type(%s) isn't REDIS_REPLY_ARRAY", REPLY_TYPE[m_type]);
        return *this;
    }

    m_element.push_back(rr);

    return *this;
}

