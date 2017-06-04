/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @file   Connection.h
 * @brief 
 * @version 1.0
 * @bug none
 * @warning none
 * @other none
 * @author chen.tengfei
 * @data   2017-05-24 19:52:07
 */
#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <stdint.h>
#include <string>
#include <vector>

class redisReply;
class socket_client;

class Connection
{
public:
    Connection(const std::string& host, uint16_t port);
    virtual ~Connection();

    bool sendCommand(const std::vector<std::string>& argv);
    redisReply* receiveReply();

private:
    void connect();

private:
    std::string m_host;
    uint16_t m_port;
    socket_client* m_socket;
};

#endif /* __CONNECTION_H__ */

