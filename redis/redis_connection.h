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
     *   ���м��ܱ����� redis-server �����������֤. redis ���������ڿͻ���ִ��
     *   commands����ǰ��Ҫͨ��������֤. ͨ���޸������ļ��� requirepass ָ��Ϳ�
     *   ����������.
     * @param [IN] password {const std::string} �� redis �����ļ���ָ������֤����
     * @return {bool}
     *    true: ����ƥ��, ����˿��Խ��պ�������
     *   false: 1) �ͻ���û����������
     *          2) ƥ��ʧ��, �ͻ�����Ҫ�����µ�����
     * @author chen.tengfei
     * @date 2017-06-06
     */
    bool auth(const std::string password);

    /**
     * @description
     *   ����˽��᷵�� message.
     * @param [IN] message {const std::string} 
     * @return {std::string} ����˷��ص���Ϣ
     * @author chen.tengfei
     * @date 2017-06-06
     */
    std::string echo(const std::string message);

    /**
     * @description
     *   ���������������һ�������Ƿ��ǿ��õ�, ������������һ�����ӵ���ʱ.
     * @param [IN] message {const std::string*} ���͸�����˵���Ϣ, Ϊ��ʱ, ����
     *   �˷��� PONG
     * @return {std::string} ����˷��ص���Ϣ
     * @author chen.tengfei
     * @date 2017-06-06
     */
    std::string ping(const std::string* message);
    bool ping();

    /**
     * @description
     *   ����������ر�����.
     * @return {bool} ʼ�շ��� true.
     * @author chen.tengfei
     * @date 2017-06-06
     */
    bool quit();

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

