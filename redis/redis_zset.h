#ifndef __REDIS_ZSET_H__
#define __REDIS_ZSET_H__

#include "redis_commamd.h"

class redis_client;

class redis_zset : public redis_command
{
public:
    redis_zset(redis_client* client):redis_command(client){}
    ~redis_zset(){}

public:
    long long zadd(const std::string key, std::map<double, std::string>& score_mems);
    
};

#endif /* __REDIS_ZSET_H__ */
