#ifndef __REDIS_SET_H__
#define __REDIS_SET_H__

#include <string>
#include <vector>

#include "redis_command.h"

class redis_client;

class redis_set : public redis_command
{
public:
    redis_set(redis_client* client):redis_command(client){}
    ~redis_set(){}

public:
    long long sadd(const std::string key, const std::vector<std::string>& member);
    long long scard(const std::string key);
    bool sdiff(const std::vector<std::string>& keys, std::vector<std::string>& result);
    bool sinter(const std::vector<std::string>& keys, std::vector<std::string>& result);
    bool sunion(const std::vector<std::string>& keys, std::vector<std::string>& result);
    long long sdiffstore(const std::string dest, const std::vector<std::string>& keys);
    long long sinterstore(const std::string dest, const std::vector<std::string>& keys);
    long long sunionstore(const std::string dest, const std::vector<std::string>& keys);
    bool sismember(const std::string key, const std::string member);
    bool smember(const std::string key, std::vector<std::string>& result);    
    bool smove(const std::string src, const std::string dest, const std::string member);
    bool spop(const std::string key, int count = -1, std::vector<std::string>& result);
    bool srandmember(const std::string key, int count = -1, std::vector<std::string>& result);
    long long srem(const std::string key, const std::vector<std::string>& member);
    bool sscan(std::string key, int& cursor, std::vector<std::string>& result);    
};

#endif /* __REDIS_SET_H__ */
