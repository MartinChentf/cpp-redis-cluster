#ifndef __REIDS_LIST_H__
#define __REIDS_LIST_H__

#include <vector>
#include <string>

#include "redis_command.h"

class redis_client;

class redis_list : public redis_command
{
public:
    redis_list(redis_client* client):redis_command(client){}
    ~redis_list(){}

public:
    std::string lindex(std::string key, int index);
    long long linsert(std::string key, bool is_before, std::string pivot, std::string value);
    long long llen(std::string key);
    bool lrange(std::string key, int start, int stop, std::vector<std::string>& result);
    long long lrem(std::string key, int count, std::string value);
    bool lset(std::string key, int index, std::string value);
    bool ltrim(std::string key, int start, int stop);

    std::string lpop(std::string key);
    long long lpush(std::string key, std::vector<std::string>& values);
    long long lpushx(std::string key, std::string value);
    std::string rpop(std::string key);
    long long rpush(std::string key, std::vector<std::string>& values);
    long long rpushx(std::string key, std::string value);
    std::string rpoplpush(std::string src, std::string dest);
    //blpop
    //brpop
    std::string brpoplpush(std::string src, std::string dest, long long timeout);
    
};

#endif /* __REIDS_LIST_H__ */
