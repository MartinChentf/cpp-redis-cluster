#include <iostream>
#include <string.h>

#include "redis_client.h"
#include "redis_string.h"

extern uint16_t crc16(const char *buf, int len);
extern 


bool redis_string::set(std::string key, std::string value)
{
    std::string command = "SET " + key + " " + value;

    redisContext* rcon = m_client->get_redis_context_by_key(key);

    if (rcon == NULL) {
        std::cout << "Can't get rediscontext by key(" << key << ")" << std::endl;
        return false;
    }

    redisReply* reply = (redisReply*)redisCommand(rcon, command.c_str());

    if (reply->type == REDIS_REPLY_ERROR) {
        std::cout << "ERROR:" << reply->str << ", command is [" << command << "]" << std::endl;
        return false;
    }
    else if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0)
    {
        return true;
    }

    return true;
}

std::string redis_string::get(std::string key)
{
    std::string command = "GET " + key;

    redisContext* rcon = m_client->get_redis_context_by_key(key);

    if (rcon == NULL) {
        std::cout << "Can't get rediscontext by key(" << key << ")" << std::endl;
        return "";
    }

    redisReply* reply = (redisReply*)redisCommand(rcon, command.c_str());

    if (reply->type == REDIS_REPLY_ERROR) {
        std::cout << "ERROR:" << reply->str << ", command is [" << command << "]" << std::endl;
        return "";
    }
    else if (reply->type == REDIS_REPLY_STRING) {
        return reply->str;
    }
    else if (reply->type == REDIS_REPLY_NIL) {
        return "";
    }

    return "";
}

/*
string redis::getrange(string key,int start,int end)
{
    char ch[1024] = {0};
    snprintf(ch, sizeof(ch)/sizeof(char), "GETRANGE %s %d %d", key.c_str(), start, end);
    string command = ch;

    m_reply = (redisReply*)redisCommand(m_rcon, command.c_str());

    if (m_reply->type == REDIS_REPLY_ERROR) {
        cout << "ERROR:" << m_reply->str << ", command is [" << command << "]" << endl;
        return "";
    }
    else if (m_reply->type == REDIS_REPLY_STRING) {
        return m_reply->str;
    }
    return "";
}

llong redis::setrange(string key,int offset,string value)
{
    char ch[1024] = {0};
    snprintf(ch, sizeof(ch)/sizeof(char), "SETRANGE %s %d %s", key.c_str(), offset, value.c_str());
    string command = ch;

    m_reply = (redisReply*)redisCommand(m_rcon, command.c_str());

    if (m_reply->type == REDIS_REPLY_ERROR) {
        cout << "ERROR:" << m_reply->str << ", command is [" << command << "]" << endl;
        return false;
    }
    else if (m_reply->type == REDIS_REPLY_INTEGER) {
        return m_reply->integer;
    }
    return false;
}

string redis::getSet(string key,string value)
{
    string command = "GETSET " + key + " " + value;

    m_reply = (redisReply*)redisCommand(m_rcon, command.c_str());

    if (m_reply->type == REDIS_REPLY_ERROR) {
        cout << "ERROR:" << m_reply->str << ", command is [" << command << "]" << endl;
        return "";
    }
    else if (m_reply->type == REDIS_REPLY_STRING) {
        return m_reply->str;
    }
    else if (m_reply->type == REDIS_REPLY_NIL) {
        return "";
    }
}

string redis::getError()
{
    if (m_reply->type == REDIS_REPLY_ERROR) {
        return m_reply->str;
    }


*/
