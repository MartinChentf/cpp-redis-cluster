#ifndef __REIDS_LIST_H__
#define __REIDS_LIST_H__

#include <vector>
#include <string>

#include "redis_command.h"

class redis_client;

class redis_list : public redis_command
{
public:
    redis_list(redis_client* client):redis_command(client){}
    ~redis_list(){}

public:    
    //blpop
    //brpop
    std::string brpoplpush(std::string src, std::string dest, long long timeout);
    /**
     * @description
     *   ����key�������б����±�Ϊindex��Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] index {int} �±�ֵ
     * @param [OUT] result {std::string&} �洢���
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����±�Ϊindex��Ԫ��
     *    0: �±곬����Χ
     *   -1: �����key��value���ʹ���(non-list)
     */
    int lindex(const std::string& key, int index, std::string& result);

    /**
     * @description
     *   ��key�������б��н�һ����Ԫ��value��ӵ�Ԫ��pivot֮ǰ/��
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] is_before {bool} ָ�����뵽Ԫ��pivot֮ǰ/��
     * @param [IN] pivot {const std::string&} ָ����Ԫ��
     * @param [IN] value {const std::string&} ��Ԫ��
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *    0: key������, �޲���
     *   -1: 1) key��value���ʹ���(non-list)
     *       2) Ԫ��pivot�����б���
     */
    long long linsert(const std::string& key, bool is_before,
                      const std::string& pivot, const std::string& value);

    /**
     * @description
     *   ����key�������б�ĳ���
     * @param [IN] key {const std::string&} �б�����key
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: �б���
     *    0: key������
     *   -1: key��value���ʹ���(non-list)
     */
    long long llen(const std::string& key);

    /**
     * @description
     *   ����������key�������б��еĵ�һ��Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [OUT] result {std::string&} �洢������Ԫ��
     * @return {long long} ���ز������, ����ֵ����:
     *    1: �ɹ������б���Ԫ��
     *    0: key������
     *   -1: key��value���ʹ���(non-list)
     */
    int lpop(const std::string& key, std::string& result);

    /**
     * @description
     *   ��key�������б�ͷ�����β���values�����е�Ԫ��, ������Ԫ��˳���values
     *   �е��෴. ���key������, ����Ԫ��ǰ�ȴ���һ�����б�
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] values {const std::vector<std::string>&} ��Ԫ�ؼ���
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *   -1: �����key��value���ʹ���(non-list)
     */
    long long lpush(const std::string& key,
                    const std::vector<std::string>& values);

    /**
     * @description
     *   ���key���ڲ������б����, ����key�������б�ͷ������Ԫ��value. ���key
     *   ������, �򲻲���Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] value {const std::string&} ��Ԫ��
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *   -1: �����key��value���ʹ���(non-list)
     */
    long long lpushx(const std::string& key, const std::string& value);

    /**
     * @description
     *   ����key�������б���[start, end](������)��Χ�ڵ�Ԫ��. start��end��Ϊ����,
     *   -1��ʾ���һ��Ԫ��, -2��ʾ�����ڶ���Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] end {int} �����±�
     * @param [OUT] result {std::vector<std::string>&} �洢ָ�������Ԫ�ؼ���
     * @return {bool} �����Ƿ�ɹ�, false��ʾ�����key��value���ʹ���(non-list)
     */
    bool lrange(const std::string& key, int start, int stop,
                std::vector<std::string>& result);

    long long lrem(std::string key, int count, std::string value);
    bool lset(std::string key, int index, std::string value);
    bool ltrim(std::string key, int start, int stop);

    std::string rpop(std::string key);
    std::string rpoplpush(std::string src, std::string dest);
    long long rpush(std::string key, std::vector<std::string>& values);
    long long rpushx(std::string key, std::string value);
    
};

#endif /* __REIDS_LIST_H__ */
