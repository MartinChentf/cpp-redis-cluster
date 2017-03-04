#ifndef __REDIS_HASH_H__
#define __REDIS_HASH_H__

#include <map>

#include "redis_command.h"

class redis_client;

class redis_hash : public redis_command
{
public:
    redis_hash(redis_client* client):redis_command(client){}
    ~redis_hash(){}

public:
    long long hdel(std::string key, std::vector<std::string>& fields);
    bool hexists(std::string key, std::string field);
    std::string hget(std::string key, std::string field);
    bool hgetall(std::string key, std::map<std::string, std::string>& result);
    long long hincrby(std::string key, std::string field, long long increment);
    std::string hincrbyfloat(std::string key, std::string field, double increment);
    bool hkeys(std::string key, std::vector<std::string>& result);
    long long hlen(std::string key);
    bool hmget(std::string key, std::vector<std::string>& field, std::vector<std::string>& result);
    bool hset(std::string key, std::map<std::string, std::string>& result);
    bool hscan(std::string key, int& cursor, std::map<std::string, std::string>& result);
    bool hset(std::string key, std::string field, std::string value);
    bool hsetnx(std::string key, std::string field, std::string value);
    long long hstrlen(std::string key, std::string field);
    bool hvals(std::string key, std::vector<std::string>& result);
};

#endif /* __REDIS_HASH_H__ */
