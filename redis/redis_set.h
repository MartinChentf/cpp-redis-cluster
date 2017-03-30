#ifndef __REDIS_SET_H__
#define __REDIS_SET_H__

#include <string>
#include <vector>

#include "redis_command.h"

class redis_client;

class redis_set : public redis_command
{
public:
    redis_set(redis_client* client):redis_command(client){}
    ~redis_set(){}

public:
    /**
     * @description
     *   ��key�����ļ��������ָ���ĳ�Ա. ���ָ���ĳ�Ա�Ѿ�����, �����. ���
     *   key������, �ȴ���һ���µļ���.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [IN] member {const std::vector<std::string>&} ָ����ӵĳ�Ա�б�
     * @return {long long} ���ز������, ����ֵ����:
     *   >=0: �����ӵ�Ԫ�ظ���, �������Ѿ����ڵļ���Ԫ��
     *    -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sadd(const std::string& key,
                   const std::vector<std::string>& member);

    /**
     * @description
     *   ����key�����ļ��ϵĻ���(���ϵ�Ԫ�ظ���)
     * @param [IN] key {const std::string&} ���϶����key
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ���ϵĻ���
     *    0: key������
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long scard(const std::string& key);

    /**
     * @description
     *   ����ָ����һ�鼯���е�һ�����������༯�ϵĲ. �����ڵ�key�������ռ�.
     * @param [IN] keys {const std::vector<std::string>&} ָ����һ�鼯��
     * @param [OUT] result {std::vector<std::string>&} �洢���صĲ
     * @return {bool} ���ز������, false��ʾ�����keys��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sdiff(const std::vector<std::string>& keys,
               std::vector<std::string>& result);

    /**
     * @description
     *   �������и������ϵĽ���. �����ڵ�key�������ռ�.
     * @param [IN] keys {const std::vector<std::string>&} ָ����һ�鼯��
     * @param [OUT] result {std::vector<std::string>&} �洢���صĽ���
     * @return {bool} ���ز������, false��ʾ�����keys��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sinter(const std::vector<std::string>& keys,
                std::vector<std::string>& result);

    /**
     * @description
     *   �������и������ϵĲ���. �����ڵ�key�������ռ�.
     * @param [IN] keys {const std::vector<std::string>&} ָ����һ�鼯��
     * @param [OUT] result {std::vector<std::string>&} �洢���صĲ���
     * @return {bool} ���ز������, false��ʾ�����keys��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sunion(const std::vector<std::string>& keys,
                std::vector<std::string>& result);

    /**
     * @description
     *   ����ָ����һ�鼯���е�һ�����������༯�ϵĲ, ���ѽ���洢�ڼ���dest
     *   ��. ���dest�����Ѵ���, �򸲸�ԭ��Ԫ��. �����ڵ�key�������ռ�.
     * @param [IN] dest {const std::string&} ��ż������ļ��϶����key
     * @param [IN] keys {const std::vector<std::string>&} ָ����һ�鼯��
     * @return {long long} ���ؽ������Ԫ�ظ���, ����ֵ����:
     *   >=0: �������Ԫ�ظ���
     *    -1: ��ʾ����򼯺϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sdiffstore(const std::string& dest,
                         const std::vector<std::string>& keys);

    /**
     * @description
     *   �������и������ϵĽ���, ���ѽ���洢�ڼ���dest��. ���dest�����Ѵ���,
     *   �򸲸�ԭ��Ԫ��. �����ڵ�key�������ռ�.
     * @param [IN] dest {const std::string&} ��ż������ļ��϶����key
     * @param [IN] keys {const std::vector<std::string>&} ָ����һ�鼯��
     * @return {long long} ���ؽ������Ԫ�ظ���, ����ֵ����:
     *   >=0: �������Ԫ�ظ���
     *    -1: ��ʾ����򼯺϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sinterstore(const std::string& dest,
                          const std::vector<std::string>& keys);

    /**
     * @description
     *   �������и������ϵĲ���, ���ѽ���洢�ڼ���dest��. ���dest�����Ѵ���,
     *   �򸲸�ԭ��Ԫ��. �����ڵ�key�������ռ�.
     * @param [IN] dest {const std::string&} ��ż������ļ��϶����key
     * @param [IN] keys {const std::vector<std::string>&} ָ����һ�鼯��
     * @return {long long} ���ؽ������Ԫ�ظ���, ����ֵ����:
     *   >=0: �������Ԫ�ظ���
     *    -1: ��ʾ����򼯺϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sunionstore(const std::string& dest,
                          const std::vector<std::string>& keys);

    bool sismember(const std::string key, const std::string member);
    bool smember(const std::string key, std::vector<std::string>& result);
    bool smove(const std::string src, const std::string dest, const std::string member);
    bool spop(const std::string key, std::vector<std::string>& result, int count = -1);
    bool srandmember(const std::string key, std::vector<std::string>& result, int count = -1);
    long long srem(const std::string key, const std::vector<std::string>& member);
    bool sscan(std::string key, int& cursor, std::vector<std::string>& result);

private:
    bool set_operation(const char* op, const std::vector<std::string>& keys,
                       std::vector<std::string>& result);
    long long set_operation_with_store(const char * op, const std::string& dest,
                                       const std::vector<std::string>& keys);
};

#endif /* __REDIS_SET_H__ */
