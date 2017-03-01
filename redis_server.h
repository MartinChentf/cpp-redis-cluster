#ifndef __REDIS_SERVER_H__
#define __REDIS_SERVER_H__


#include "redis_command.h"

class redis_client;

class redis_server : public redis_command
{
public:
    redis_server(redis_client* client):redis_command(client){}
    ~redis_server(){}

public:
    bool flushall();
};

#endif /* __REDIS_SERVER_H__ */
