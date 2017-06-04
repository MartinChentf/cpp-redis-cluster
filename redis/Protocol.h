/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @file   Protocol.h
 * @brief 
 * @version 1.0
 * @bug none
 * @warning none
 * @other none
 * @author chen.tengfei
 * @data   2017-05-20 16:09:24
 */
#ifndef __REDIS_PROTOCOL_H__
#define __REDIS_PROTOCOL_H__

#include <string>
#include <vector>

#include "redisReply.h"

class socket_client;

class Protocol
{
public:
    /**
     * @description
     *   发送请求命令
     * @param [IN] socket {socket_client*} 连接redis_server的socket客户端
     * @param [IN] argv {const std::vector<std::string>&} 请求的命令及其参数
     * @return {bool} 发送是否成功
     * @author chen.tengfei
     * @date 2017-04-20
     */
    static bool sendCommand(socket_client* socket,
        const std::vector<std::string>& argv);

    /**
     * @description
     *   读取并解析redis_server的应答, 并将解析后的数据存入redis_reply中
     * @param [IN] socket {socket_client*} 连接redis_server的socket客户端
     * @return {redisReply*} 解析后的redis_server的应答, 返回值由调用方负责释放
     * @author chen.tengfei
     * @date 2017-05-26
     */
    static redisReply* process(socket_client* socket);

private:
    static std::string build_request(const std::vector<std::string>& argv);

    static redisReply* processItem(socket_client* socket);
    static redisReply* processLineItem(socket_client* socket,
        t_redis_reply type);
    static redisReply* processError(socket_client* socket);
    static redisReply* processStatus(socket_client* socket);
    static redisReply* processInteger(socket_client* socket);
    static redisReply* processBulkItem(socket_client* socket);
    static redisReply* processMultiBulkItem(socket_client* socket);
};

#endif /* __REDIS_PROTOCOL_H__ */

