#include <string.h>
#include <stdlib.h>

#include "redis_command.h"
#include "redis_client.h"
#include "redis_helper.h"
#include "redis_log.h"


redis_command::redis_command(redis_client * client)
: m_client(client)
, m_rcon(NULL)
, m_command("")
, m_slot(0)
{}

void redis_command::set_client(redis_client * client)
{
    if (client != NULL) {
        m_client = client;
    }
}

void redis_command::hash_slots(std::string key)
{
    if (m_client != NULL) {
        m_slot = m_client->get_key_slot(key);
        m_rcon = m_client->get_redis_context_by_slot(m_slot);
    }
}

std::string redis_command::build_command(const char * format,...)
{
    char cmd[10240] = {0};

    va_list ap;
    va_start(ap,format);
    vsnprintf(cmd, sizeof(cmd), format, ap);
    va_end(ap);

    m_command = cmd;

    return m_command;
}

bool redis_command::is_normal_context()
{
    if (m_rcon == NULL || m_rcon->err) {
        return false;
    }
    return true;
}

void redis_command::confirm_redis_context()
{
    if (m_client != NULL) {
        if (!is_normal_context()) {
            m_rcon = m_client->get_redis_context();
        }
    }
    else {
        m_rcon = NULL;
    }
}

redisReply* redis_command::run_command()
{
    confirm_redis_context();
    if (!is_normal_context()) {
        ERROR("redisContext is abnormal, Error: %s",
            m_rcon ? m_rcon->errstr : "redisContext is NULL");
        return NULL;
    }

    redisReply* reply = (redisReply*)redisCommand(m_rcon, m_command.c_str());

    return reply;
}

std::string redis_command::parse_reply(redisReply * reply)
{
    if (reply == NULL) {
        return "Reply is NULL";
    }

    std::string result("Reply Type: ");
    result += REPLY_TYPE[reply->type];
    switch(reply->type) {
        case REDIS_REPLY_STATUS:
            return result + ", Status: " + reply->str;
        case REDIS_REPLY_NIL:
            return result;
        case REDIS_REPLY_STRING:
            return result + ", String: " + reply->str;
        case REDIS_REPLY_ERROR:
            return result + ", Errstr: " + reply->str;
        case REDIS_REPLY_INTEGER:
            return result + ", Integer: " + redis_helper::to_string(reply->integer);
        case REDIS_REPLY_ARRAY:
            return result + ", Array Elements:" + redis_helper::to_string(reply->elements);
        default:
            return "Unkonw Type";
    }
}

bool redis_command::check_status()
{
    bool bret = false;

    redisReply* reply = run_command();

    if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
    }
    else if (reply->type == REDIS_REPLY_STATUS &&
             strcasecmp(reply->str, "OK") == 0) {
        NORMAL("Execute command success! [%s]", m_command.c_str());
        bret = true;
    }
    else {
        WARN("Unexpected reply: %s", parse_reply(reply).c_str());
    }

    freeReplyObject(reply);
    return bret;
}

bool redis_command::check_status_or_nil()
{
    bool bret = false;

    redisReply* reply = run_command();

    if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
    }
    else if (reply->type == REDIS_REPLY_STATUS &&
             strcasecmp(reply->str, "OK") == 0) {
        NORMAL("Execute command success! [%s]", m_command.c_str());
        bret = true;
    }
    else if (reply->type == REDIS_REPLY_NIL) {
        NORMAL("Reply is nil! [%s]");
        bret = true;
    }
    else {
        WARN("Unexpected reply: %s", parse_reply(reply).c_str());
    }

    freeReplyObject(reply);
    return bret;
}

std::string redis_command::get_string()
{
    std::string ret_str("");

    redisReply* reply = run_command();

    if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
    }
    else if (reply->type == REDIS_REPLY_STRING) {
        NORMAL("Execute command success! [%s]", m_command.c_str());
        ret_str = reply->str;
    }
    else {
        WARN("Unexpected reply: %s", parse_reply(reply).c_str());
    }

    freeReplyObject(reply);
    return ret_str;
}

std::string redis_command::get_string_or_nil()
{
    std::string ret_str("");

    redisReply* reply = run_command();

    if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
    }
    else if (reply->type == REDIS_REPLY_STRING) {
        NORMAL("Execute command success! [%s]", m_command.c_str());
        ret_str = reply->str;
    }
    else if (reply->type == REDIS_REPLY_NIL) {
        NORMAL("Reply is nil! [%s]", m_command.c_str());
    }
    else {
        WARN("Unexpected reply: %s", parse_reply(reply).c_str());
    }

    freeReplyObject(reply);
    return ret_str;
}

long long redis_command::get_integer64()
{
    long long llret = -1;

    redisReply* reply = run_command();

    if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
    }
    else if (reply->type == REDIS_REPLY_INTEGER) {
        NORMAL("Execute command success! [%s]", m_command.c_str());
        llret = reply->integer;
    }
    else {
        WARN("Unexpected reply: %s", parse_reply(reply).c_str());
    }

    freeReplyObject(reply);
    return llret;
}

int redis_command::get_integer32()
{
    return get_integer64();
}

bool redis_command::get_array(std::vector<std::string> * result)
{
    bool bret = false;

    redisReply* reply = run_command();

    if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
    }
    else if (reply->type == REDIS_REPLY_ARRAY) {
        NORMAL("Execute command success! [%s]", m_command.c_str());
        for (size_t i = 0; i < reply->elements; i ++) {
            redisReply* elem = reply->element[i];
            if (elem->type == REDIS_REPLY_STRING) {
                result->push_back(elem->str);
            }
            else {
                result->push_back("");
            }
        }
        bret = true;
    }
    else {
        WARN("Unexpected reply: %s", parse_reply(reply).c_str());
    }

    return bret;
}

bool redis_command::get_cursor_array(int& cursor, std::vector<std::string>* result)
{
    bool bret = false;

    redisReply* reply = run_command();

    if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
    }
    else if (reply->type == REDIS_REPLY_ARRAY) {
        if (reply->elements != 2) {
            ERROR("Execute command fail! [%s], array elements != 2", m_command.c_str());
        }
        NORMAL("Execute command success! [%s]", m_command.c_str());

        if (reply->element[0]->type == REDIS_REPLY_STRING) {
            cursor = atol(reply->element[0]->str);
        }
        redisReply* array = reply->element[1];
        for (size_t i = 0; i < array->elements; i ++) {
            redisReply* elem = array->element[i];
            if (elem->type == REDIS_REPLY_STRING) {
                result->push_back(elem->str);
            }
            else {
                result->push_back("");
            }
        }
        bret = true;
    }
    else {
        WARN("Unexpected reply: %s", parse_reply(reply).c_str());
    }

    return bret;
}

