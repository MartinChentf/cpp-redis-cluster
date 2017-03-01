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
/*    bool msetnx(vector<pair<string, string>>& keyValues);
    bool setex(string key, llong second, string value);
    bool pSetex(string key, llong millisecond, string value);
    bool setnx(string key, string string value);
    llong strlen(string key);
    llong incr(string key);
    llong incrBy(string key, int increment);
    string incrByFloat(string key, float increment);
    llong decr(string key);
    llong decrBy(string key, int decrement);
    llong append(string key, string value);*/
};

#endif /* __REDIS_STRING_H__ */
