#include <stdlib.h>

#include "redis_command.h"
#include "redis_helper.h"
#include "redis_log.h"
#include "redisReply.h"

redis_command::redis_command(const std::string & host, uint16_t port)
: Connection(host, port)
, m_command("")
{}

std::string redis_command::parse_reply(const redisReply* reply)
{
    if (reply == NULL) {
        return "Reply is NULL";
    }

    std::string result("Reply Type: ");
    result += REPLY_TYPE[reply->get_type()];
    switch(reply->get_type()) {
        case REDIS_REPLY_STATUS:
            return result + ", Status: " + reply->get_status();
        case REDIS_REPLY_NIL:
            return result;
        case REDIS_REPLY_STRING:
            return result + ", String: " + reply->get_string();
        case REDIS_REPLY_ERROR:
            return result + ", Errstr: " + reply->get_error();
        case REDIS_REPLY_INTEGER:
            return result + ", Integer: "
                    + TO_STRING(reply->get_integer());
        case REDIS_REPLY_ARRAY:
            return result + ", Array Elements:"
                    + TO_STRING(reply->get_size());
        default:
            return "Unkonw Type";
    }
}

bool redis_command::check_status(const char * expection /*= "OK"*/)
{
    const redisReply* reply = receiveReply();
    if (reply == NULL || reply->get_type() != REDIS_REPLY_STATUS) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return false;
    }

    const std::string status = reply->get_status();
    if (status.empty()) {
        WARN("Execute command fail! , status is empty");
        SAFE_DELETE(reply);
        return false;
    }
    else if (expection == NULL || status.compare(expection) == 0) {
        DEBUG("Execute command success! [%s]", m_command.c_str());
        SAFE_DELETE(reply);
        return true;
    }
    else {
        WARN("Execute command fail! , status:[%s], expection:[%s]",
             status.c_str(), expection);
        SAFE_DELETE(reply);
        return false;
    }
}

int redis_command::check_status_or_nil(const char * expection /*= "OK"*/)
{
    redisReply* reply = receiveReply();
    if (reply == NULL
        || (reply->get_type() != REDIS_REPLY_STATUS
            && reply->get_type() != REDIS_REPLY_NIL)) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return -1;
    }

    if (reply->get_type() == REDIS_REPLY_NIL) {
        DEBUG("Execute command success! [%s], Reply is nil!",
            m_command.c_str());
        SAFE_DELETE(reply);
        return 0;
    }

    const std::string status = reply->get_status();
    if (status.empty()) {
        WARN("Execute command fail! , status is empty");
        SAFE_DELETE(reply);
        return false;
    }
    else if (expection == NULL || status.compare(expection) == 0) {
        DEBUG("Execute command success! [%s]", m_command.c_str());
        SAFE_DELETE(reply);
        return 1;
    }
    else {
        WARN("Execute command fail! , status:[%s], expection:[%s]",
             status.c_str(), expection);
        SAFE_DELETE(reply);
        return -1;
    }
}

std::string redis_command::get_status()
{
    const redisReply* reply = receiveReply();
    if (reply == NULL || reply->get_type() != REDIS_REPLY_STATUS) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return "";
    }

    const std::string status = reply->get_status();
    if (status.empty()) {
        WARN("Execute command fail! , status is empty");
        SAFE_DELETE(reply);
        return "";
    }
    DEBUG("Execute command success! [%s]", m_command.c_str());
    SAFE_DELETE(reply);

    return status;
}

bool redis_command::get_string(std::string & result)
{
    return get_string(&result);
}

bool redis_command::get_string(std::string * result)
{
    redisReply* reply = receiveReply();
    if (reply == NULL || reply->get_type() != REDIS_REPLY_STRING) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return false;
    }
    DEBUG("Execute command success! [%s]", m_command.c_str());

    if (result != NULL)
        *result = reply->get_string();
    SAFE_DELETE(reply);
    return true;
}

int redis_command::get_string_or_nil(std::string& result)
{
    redisReply* reply = receiveReply();
    if (reply == NULL
        || (reply->get_type() != REDIS_REPLY_STRING
            && reply->get_type() != REDIS_REPLY_NIL)) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return -1;
    }

    if (reply->get_type() == REDIS_REPLY_NIL) {
        DEBUG("Execute command success! [%s], Reply is nil!",
            m_command.c_str());
        SAFE_DELETE(reply);
        return 0;
    }
    DEBUG("Execute command success! [%s]", m_command.c_str());

    result = reply->get_string();
    SAFE_DELETE(reply);
    return 1;
}

long long redis_command::get_integer64(bool * success /*= NULL*/)
{
    long long llret = -1;

    redisReply* reply = receiveReply();
    if (reply == NULL || reply->get_type() != REDIS_REPLY_INTEGER) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_ASSIGN(success, false);
        SAFE_DELETE(reply);
        return -1;
    }
    DEBUG("Execute command success! [%s]", m_command.c_str());

    llret = reply->get_integer();
    SAFE_ASSIGN(success, true);
    SAFE_DELETE(reply);
    return llret;
}

bool redis_command::get_integer64(long long & result)
{
    bool success;
    result = get_integer64(&success);
    return success;
}

bool redis_command::get_integer64(long long * result)
{
    bool success;
    SAFE_ASSIGN_FUNC(result, get_integer64(&success));
    return success;
}

int redis_command::get_integer32(bool * success /*= NULL*/)
{
    return get_integer64(success);
}

long long redis_command::get_integer64_or_nil(bool * success /*= NULL*/)
{
    long long llret = -1;

    redisReply* reply = receiveReply();
    if (reply == NULL
        || (reply->get_type() != REDIS_REPLY_INTEGER
            && reply->get_type() != REDIS_REPLY_NIL)) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_ASSIGN(success, false);
        SAFE_DELETE(reply);
        return -1;
    }

    if (reply->get_type() == REDIS_REPLY_NIL) {
        DEBUG("Execute command success! [%s], Reply is nil!",
            m_command.c_str());
        SAFE_ASSIGN(success, true);
        SAFE_DELETE(reply);
        return 0;
    }
    DEBUG("Execute command success! [%s]", m_command.c_str());
    llret = reply->get_integer();
    SAFE_ASSIGN(success, true);
    SAFE_DELETE(reply);

    return llret;
}

int redis_command::get_integer32_or_nil(bool * success /*= NULL*/)
{
    return get_integer64_or_nil(success);
}

int redis_command::get_array(std::vector<std::string*>& result)
{
    redisReply* reply = receiveReply();
    if (reply == NULL || reply->get_type() != REDIS_REPLY_ARRAY) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return -1;
    }
    DEBUG("Execute command success! [%s]", m_command.c_str());

    size_t count = reply->get_size();
    for (size_t i = 0; i < count; i ++) {
        const redisReply* elem = reply->get_element(i);
        if (elem->get_type() == REDIS_REPLY_STRING) {
            result.push_back(new std::string(elem->get_string()));
        }
        else {
            result.push_back(NULL);
        }
    }
    SAFE_DELETE(reply);
    return count;
}

int redis_command::get_array(std::vector<std::string>& result)
{
    redisReply* reply = receiveReply();
    if (reply == NULL || reply->get_type() != REDIS_REPLY_ARRAY) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return false;
    }
    DEBUG("Execute command success! [%s]", m_command.c_str());

    size_t count = reply->get_size();
    for (size_t i = 0; i < count; i ++) {
        const redisReply* elem = reply->get_element(i);
        if (elem->get_type() == REDIS_REPLY_STRING) {
            result.push_back(elem->get_string());
        }
        else {
            result.push_back("");
        }
    }
    SAFE_DELETE(reply);
    return count;
}

int redis_command::get_array_or_nil(std::vector<std::string>& result)
{
    redisReply* reply = receiveReply();
    if (reply == NULL
        || (reply->get_type() != REDIS_REPLY_ARRAY
            && reply->get_type() != REDIS_REPLY_NIL)) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return -1;
    }
    
    if (reply->get_type() == REDIS_REPLY_NIL) {
        DEBUG("Execute command success! [%s], Reply is nil!",
            m_command.c_str());
        SAFE_DELETE(reply);
        return 0;
    }
    DEBUG("Execute command success! [%s]", m_command.c_str());

    size_t count = reply->get_size();
    for (size_t i = 0; i < count; i ++) {
        const redisReply* elem = reply->get_element(i);
        if (elem->get_type() == REDIS_REPLY_STRING) {
            result.push_back(elem->get_string());
        }
        else {
            result.push_back("");
        }
    }
    SAFE_DELETE(reply);
    return count;
}

int redis_command::get_cursor_array(std::vector<std::string>* result)
{
    int cursor = -1;
    redisReply* reply = receiveReply();

    if (reply == NULL || reply->get_type() != REDIS_REPLY_ARRAY) {
        ERROR("Execute command fail! [%s], %s",
            m_command.c_str(), parse_reply(reply).c_str());
        SAFE_DELETE(reply);
        return -1;
    }
    
    if (reply->get_size() != 2) {
        ERROR("Execute command fail! [%s], array elements != 2",
            m_command.c_str());
        SAFE_DELETE(reply);
        return -1;
    }
    DEBUG("Execute command success! [%s]", m_command.c_str());

    if (reply->get_element(0)->get_type() == REDIS_REPLY_STRING) {
        cursor = atoi(reply->get_element(0)->get_string().c_str());
    }
    if (result) {
        const redisReply* array = reply->get_element(1);
        for (size_t i = 0; i < array->get_size(); i ++) {
            const redisReply* elem = array->get_element(i);
            if (elem->get_type() == REDIS_REPLY_STRING) {
                result->push_back(elem->get_string());
            }
            else {
                result->push_back("");
            }
        }
    }
    SAFE_DELETE(reply);
    return cursor;
}

void redis_command::generate_cmdstr(const std::vector<std::string>& argv)
{
    m_command.clear();

    for (size_t i = 0; i < argv.size(); i++) {
        m_command.push_back('"');
        m_command += argv[i];
        m_command.push_back('"');
        m_command.push_back(' ');
    }
}

void redis_command::scan_keys(const char* cmd, const std::string* key,
                              int cursor, const char* pattern, int count)
{
    std::vector<std::string> argv;
    argv.push_back(cmd);

    if (key) {
        argv.push_back(*key);
    }

    argv.push_back(TO_STRING(cursor));

    if (pattern && *pattern) {
        argv.push_back("MATCH");
        argv.push_back(pattern);
    }

    if (count != 10) {
        argv.push_back("COUNT");
        argv.push_back(TO_STRING(count));
    }

    sendCommand(argv);
}

bool redis_command::sendCommand(const std::vector<std::string>& argv)
{
    generate_cmdstr(argv);
    return Connection::sendCommand(argv);
}

