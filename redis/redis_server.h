#ifndef __REDIS_SERVER_H__
#define __REDIS_SERVER_H__

#include "redis_command.h"

class redis_server : virtual public redis_command
{
public:
    redis_server(const std::string& host, uint16_t port);
    ~redis_server(){}

public:
    bool bgrewriteaof();
    bool flushall();
};

#endif /* __REDIS_SERVER_H__ */
