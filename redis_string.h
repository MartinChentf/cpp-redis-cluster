#ifndef __REDIS_STRING_H__
#define __REDIS_STRING_H__

#include <string>
//#include <boost/shared_ptr.hpp>

class redis_client;

class redis_string
{
public:
    redis_string(redis_client* client):m_client(client){}
    ~redis_string(){}


    bool set(std::string key, std::string value);
    std::string get(std::string key);
/*    string getrange(string key, int start, int end);
    llong setrange(string key, int offset, string value);
    string getSet(string key, string value);
    llong getbit(string key, int offset);
    llong setbit(string key, int offset, int value);
    vector<string> mget(vector<string>& keys);
    bool mset(vector<pair<string, string>>& keyValues);
    bool msetnx(vector<pair<string, string>>& keyValues);
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

private:
    redis_client* m_client;
};

#endif /* __REDIS_STRING_H__ */
