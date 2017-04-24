#ifndef __REDIS_KEY_H__
#define __REDIS_KEY_H__

#include <vector>
#include <string>

#include "redis_command.h"

class redis_key : public redis_command
{
public:
    redis_key(redis_client* client):redis_command(client){}
    ~redis_key(){}

public:
    /**
     * ɾ��ָ����һ����һ��keys, ���Բ����ڵ�key
     * @param [IN] key {const std::vector<std::string>&} ָ����һ��keys
     * @return {int} ����ʵ��ɾ����key�ĸ���, ����:
     *   0: δɾ���κ�key
     *  >0: ʵ��ɾ����key�ĸ���, ��ֵ����С������key�ĸ���
     *  -1: ����
     */
    int del(const std::vector<std::string>& keys);
    int del(const std::string& key);

    /**
     * @description
     *   ���л�����key��ֵ, ���������л����ֵ. ʹ��RESTORE����ɽ���ֵ�����л�
     *   ����redis key��.
     * @param [IN] key {const std::string&} ������key
     * @param [OUT] result {std::string&} �洢���л����ֵ
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-22
     */
    int dump(const std::string& key, std::string& result);

    /**
     * @description
     *   �ж�key�Ƿ����. ֧��redis 3.0.3���ϰ汾.
     * @param [IN] keys {const std::vector<std::string>&} ������key
     * @return {int} ���ش��ڵ�key�ĸ���, ����ֵ����:
     *   >0: ���ڵ�key�ĸ���, ���������ͬһ��key���ظ����, �򽫻ᱻ��μ���
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int exists(const std::vector<std::string>& keys);

    /**
     * @description
     *   �ж�key�Ƿ����.
     * @param [IN] key {const std::string&} ������key
     * @return {int} ����key�Ƿ����, ����ֵ����:
     *    1: key����
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int exists(const std::string& key);

    /**
     * @description
     *   ����key������ʱ��. ����ʱ�䳬ʱ, key���ᱻɾ��
     * @param [IN] key {const std::string&} ������key
     * @param [IN] second {int} ����ʱ��, ��λ: ��. ���ʱ��Ϊ������, key������
     *   ��ɾ��
     * @return {int} ����ֵ����:
     *    1: ���óɹ�
     *    0: key�����ڻ�����ʱ���޷�������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int expire(const std::string& key, int second);

    /**
     * @description
     *   ��UNIXʱ�������key������ʱ��.
     * @param [IN] key {const std::string&} ������key
     * @param [IN] timestamp {time_t} UNIXʱ���. ���ʱ�����ʾ��ȥ��ʱ��, key
     *   ��������ɾ��
     * @return {int} ����ֵ����:
     *    1: ���óɹ�
     *    0: key�����ڻ�ʱ����޷�������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int expireat(const std::string& key, time_t timestamp);

    /**
     * @description
     *   �������з��ϸ���ģʽpattern������key.
     *   ֧�ֵ�������ģʽ:
     *    a) * ƥ������key
     *    b) h?llo ƥ�� hello, hallo �� hxllo
     *    c) h*llo ƥ�� hllo �� heeeello
     *    d) h[ae]llo ƥ�� hello �� hallo, ���ǲ�ƥ�� hillo
     *    e) h[^e]llo ƥ�� hallo, hbllo, ... ���ǲ�ƥ�� hello
     *    f) h[a-b]llo ƥ�� hallo �� hbllo
     * @param [IN] pattern {const std::string&} ƥ��ģʽ
     * @param [OUT] result {std::vector<std::string>&} �洢�����
     * @return {int} ���ط���������key�ĸ���, ����ֵ����:
     *   >0: ����������key�ĸ���
     *    0: û�з���������key
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int keys(const std::string& pattern, std::vector<std::string>& result);

    /**
     * @description
     *   ���ڵ�����ǰѡ���redis���ݿ���key�ļ���
     * @param [IN] cursor {int} �α�ֵ, ��һ�ε���ʹ��0��Ϊ�α�.
     * @param [OUT] result {std::vector<std::string>&} �洢�����, �ڲ���׷�ӷ�
     *   ʽ�����α��������ӽ��ö�����, Ϊ��ֹ���ܽ���������¸��������, ��
     *   �����ڵ��ñ�����ǰ������ö���.
     * @param [IN] pattern {const char*} glob����ģʽ����, �ǿ�ʱ��Ч
     * @param [IN] count {int} �޶������Ԫ�ص�����, Ĭ��ֵ:10
     * @return {int} ��һ���α�λ��, ����ֵ����:
     *   >0: ��һ���α�λ��
     *    0: ��������
     *   -1: key��value���ʹ���(non-hash)
     */
    int scan(int cursor, std::vector<std::string>& result,
             const char* pattern = NULL, int count = 10);
};

#endif /* __REDIS_KEY_H__ */