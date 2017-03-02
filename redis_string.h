#ifndef __REDIS_STRING_H__
#define __REDIS_STRING_H__

#include <string>
#include <vector>
#include <map>

#include "redis_command.h"

class redis_client;

class redis_string : public redis_command
{
public:
    redis_string(redis_client* client):redis_command(client){}
    ~redis_string(){}

public:
    bool set(std::string key, std::string value);
    std::string get(std::string key);
    std::string getSet(std::string key, std::string value);

    std::string getrange(std::string key, int start, int end);
    long long setrange(std::string key, int offset, std::string value);

    long long getbit(std::string key, int offset);
    long long setbit(std::string key, int offset, int value);

    bool mget(std::vector<std::string>& keys, std::vector<std::string>& result);
    bool mget(std::vector<std::string>& keys, std::vector<std::string>* result);
    bool mset(std::map<std::string, std::string>& keyValues);
    bool msetnx(std::map<std::string, std::string>& keyValues);

    bool setnx(std::string key, std::string value);
    bool setex(std::string key, long long second, std::string value);
    bool psetex(std::string key, long long millisecond, std::string value);
/*    llong strlen(string key);
    llong incr(string key);
    llong incrBy(string key, int increment);
    string incrByFloat(string key, float increment);
    llong decr(string key);
    llong decrBy(string key, int decrement);
    llong append(string key, string value);*/
};

#endif /* __REDIS_STRING_H__ */
