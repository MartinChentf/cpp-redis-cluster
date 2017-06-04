#include <stdlib.h>

#include "Protocol.h"
#include "redis_log.h"
#include "redis_helper.h"
#include "socket_client.h"

bool Protocol::sendCommand(socket_client* socket, const std::vector<std::string>& argv)
{
    if (socket == NULL) {
        ERROR("m_socket is NULL");
        return false;
    }

    std::string request = build_request(argv);

    if (!request.empty() &&
        socket->write(request.c_str(), request.size()) == -1) {
        ERROR("send to redis(%s:%d) error, req: %s", socket->host().c_str(),
            socket->port(), request.c_str());
        return false;
    }

    return true;
}

std::string Protocol::build_request(const std::vector<std::string>& argv)
{
    std::string request;

    request += "*";
    request += TO_STRING(argv.size());
    request += "\r\n";

    for (size_t i = 0; i < argv.size(); i++) {
        request += "$";
        request += TO_STRING(argv[i].size());
        request += "\r\n";
        request += argv[i];
        request += "\r\n";
    }

    return request;
}

redisReply* Protocol::process(socket_client * socket)
{
    if (socket == NULL) {
        ERROR("socket is NULL");
        return NULL;
    }

    return processItem(socket);
}

redisReply* Protocol::processItem(socket_client * socket)
{
    char ch = socket->read_byte();
    switch (ch) {
        case '-':   // ERROR
            return processError(socket);
        case '+':   // STATUS
            return processStatus(socket);
        case ':':   // INTEGER
            return processInteger(socket);
        case '$':   // STRING
            return processBulkItem(socket);
        case '*':   // ARRAY
            return processMultiBulkItem(socket);
        default:    // INVALID
            return NULL;
    }
}

redisReply* Protocol::processLineItem(socket_client * socket,
    t_redis_reply type)
{
    std::string buff = socket->read_line();
    if (buff.size() <= 0) {
        ERROR("read line from redis(%s:%d) error", socket->host().c_str(),
            socket->port());
        return NULL;
    }

    redisReply* rr = new redisReply();
    rr->set_type(type);
    rr->put(buff);

    return rr;
}

redisReply* Protocol::processError(socket_client * socket)
{
    return processLineItem(socket, REDIS_REPLY_ERROR);
}

redisReply* Protocol::processStatus(socket_client * socket)
{
    return processLineItem(socket, REDIS_REPLY_STATUS);
}

redisReply* Protocol::processInteger(socket_client * socket)
{
    return processLineItem(socket, REDIS_REPLY_INTEGER);
}

redisReply* Protocol::processBulkItem(socket_client * socket)
{
    std::string str_buff = socket->read_line();
    if (str_buff.size() <= 0) {
        ERROR("read line from redis(%s:%d) error", socket->host().c_str(),
            socket->port());
        return NULL;
    }

    redisReply* rr = NULL;
    int len = atoi(str_buff.c_str());
    if (len < 0) {
        rr = new redisReply();
        rr->set_type(REDIS_REPLY_NIL);
    }
    else {
        char* buff = new char[len + 1];
        if (len > 0 && socket->read(buff, len) <= 0) {
            ERROR("read from redis(%s:%d) error", socket->host().c_str(),
                socket->port());
            return NULL;
        }
        buff[len] = 0;
        rr = new redisReply();
        rr->set_type(REDIS_REPLY_STRING);
        rr->put(buff, len);

        // 读取\r\n
        str_buff = socket->read_line();
    }
    return rr;
}

redisReply* Protocol::processMultiBulkItem(socket_client * socket)
{
    std::string str_buff = socket->read_line();
    if (str_buff.size() <= 0) {
        ERROR("read line from redis(%s:%d) error", socket->host().c_str(),
            socket->port());
        return NULL;
    }

    redisReply* rr = NULL;
    int count = atoi(str_buff.c_str());
    if (count <= 0) {
        rr = new redisReply();
        rr->set_type(REDIS_REPLY_NIL);
    }
    else {
        rr = new redisReply();
    	rr->set_type(REDIS_REPLY_ARRAY);
        for (int i = 0; i < count; i++) {
            redisReply* element = processItem(socket);
            if (element == NULL) {
                SAFE_DELETE(rr);  // 释放整个数组, 防止内存泄漏
                return NULL;
            }
            rr->put(element);
        }
    }
    return rr;
}

