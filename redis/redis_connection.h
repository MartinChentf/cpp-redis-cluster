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
     *   选择一个数据库. 数据库下标从0开始. 非集群模式的server实例有16个数据库,
     *   即1~15. 集群模式的server实例只有一个数据库. 该命令不允许在集群模式下使
     *   用.
     * @param [IN] index {int} 数据库下标, 基于0
     * @return {bool} 返回操作结果, true表示数据库切换成功
     * @author chen.tengfei
     * @date 2017-06-04
     */
    bool select(int index);
};

#endif /* __REDIS_CONNECTION_H__ */

