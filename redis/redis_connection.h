/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @file   redis_connection.h
 * @brief 
 * @version 1.0
 * @bug none
 * @warning none
 * @other none
 * @author chen.tengfei
 * @data   2017-06-04 17:01:23
 */
#ifndef __REDIS_CONNECTION_H__
#define __REDIS_CONNECTION_H__

#include <string>

#include "redis_command.h"

class redis_connection : virtual public redis_command
{
public:
    redis_connection(const std::string& host, uint16_t port);
    ~redis_connection(){}

public:
    /**
     * @description
     *   ѡ��һ�����ݿ�. ���ݿ��±��0��ʼ. �Ǽ�Ⱥģʽ��serverʵ����16�����ݿ�,
     *   ��1~15. ��Ⱥģʽ��serverʵ��ֻ��һ�����ݿ�. ����������ڼ�Ⱥģʽ��ʹ
     *   ��.
     * @param [IN] index {int} ���ݿ��±�, ����0
     * @return {bool} ���ز������, true��ʾ���ݿ��л��ɹ�
     * @author chen.tengfei
     * @date 2017-06-04
     */
    bool select(int index);
};

#endif /* __REDIS_CONNECTION_H__ */

