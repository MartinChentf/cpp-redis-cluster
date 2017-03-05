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
    long long zcard(const std::string key);
    long long zcount(const std::string key, double min, double max);
    long long zcount(const std::string key, std::string min, std::string max);
    double zincrby(const std::string key, double increment, std::string member);
};

#endif /* __REDIS_ZSET_H__ */
