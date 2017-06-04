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
     *   ������������
     * @param [IN] socket {socket_client*} ����redis_server��socket�ͻ���
     * @param [IN] argv {const std::vector<std::string>&} �������������
     * @return {bool} �����Ƿ�ɹ�
     * @author chen.tengfei
     * @date 2017-04-20
     */
    static bool sendCommand(socket_client* socket,
        const std::vector<std::string>& argv);

    /**
     * @description
     *   ��ȡ������redis_server��Ӧ��, ��������������ݴ���redis_reply��
     * @param [IN] socket {socket_client*} ����redis_server��socket�ͻ���
     * @return {redisReply*} �������redis_server��Ӧ��, ����ֵ�ɵ��÷������ͷ�
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

