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
     *   在有加密保护的 redis-server 上请求身份认证. redis 可以设置在客户端执行
     *   commands请求前需要通过密码验证. 通过修改配置文件的 requirepass 指令就可
     *   以设置密码.
     * @param [IN] password {const std::string} 在 redis 配置文件中指定的认证密码
     * @return {bool}
     *    true: 密码匹配, 服务端可以接收后续命令
     *   false: 1) 客户端没有设置密码
     *          2) 匹配失败, 客户端需要重试新的密码
     * @author chen.tengfei
     * @date 2017-06-06
     */
    bool auth(const std::string password);

    /**
     * @description
     *   服务端将会返回 message.
     * @param [IN] message {const std::string} 
     * @return {std::string} 服务端返回的消息
     * @author chen.tengfei
     * @date 2017-06-06
     */
    std::string echo(const std::string message);

    /**
     * @description
     *   该命令经常用来测试一个连接是否还是可用的, 或者用来测试一个连接的延时.
     * @param [IN] message {const std::string*} 发送给服务端的信息, 为空时, 服务
     *   端返回 PONG
     * @return {std::string} 服务端返回的消息
     * @author chen.tengfei
     * @date 2017-06-06
     */
    std::string ping(const std::string* message);
    bool ping();

    /**
     * @description
     *   请求服务器关闭连接.
     * @return {bool} 始终返回 true.
     * @author chen.tengfei
     * @date 2017-06-06
     */
    bool quit();

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

