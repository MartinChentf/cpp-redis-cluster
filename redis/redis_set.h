#ifndef __REDIS_SET_H__
#define __REDIS_SET_H__

#include <string>
#include <vector>

#include "redis_command.h"

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
     *    -1: ����򼯺϶�������ʹ���(non-set)
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
     *    -1: ����򼯺϶�������ʹ���(non-set)
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
     *    -1: ����򼯺϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sunionstore(const std::string& dest,
                          const std::vector<std::string>& keys);

    /**
     * @description
     *   �жϸ�����Ԫ��member�Ƿ���key�������ϵĳ�Ա
     * @param [IN] key {const std::string&} ���϶����key
     * @param [IN] member {const std::string&} ������Ԫ��
     * @return {int} �����жϽ��, ����ֵ����:
     *    1: ������Ԫ���Ǽ��ϳ�Ա
     *    0: ������Ԫ�ز��Ǽ��ϳ�Ա����key������
     *   -1: ����򼯺϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int sismember(const std::string& key, const std::string& member);

    /**
     * @description
     *   ��ȡkey�����ļ��ϵ����г�Ա. Ч����ֻ��һ��keyֵ��sinter����һ��.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::vector<std::string>&} ��ŷ��ؽ��
     * @return {int} ���ز������, ����ֵ����:
     *   >0: ����Ԫ�ظ���
     *    0: key������
     *   -1: �����keys��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int smembers(const std::string& key, std::vector<std::string>& result);

    /**
     * @description
     *   �Ѹ�����Ԫ��member��src�����ļ����Ƶ�dest�����ļ���. �ò�����һ��ԭ�Ӳ�
     *   ��. �ڱ�Ŀͻ��˿���, �ڸ���ʱ��Ԫ��memberֻ������src�л�dest��.
     *   1) src���ϲ����ڻ��߲�����Ԫ��member, �����κβ���������0.
     *   2) ��Ԫ��member�Ѿ�������dest������, ��ֻ��Ԫ��member��src�������Ƴ�.
     * @param [IN] src {const std::string&} src���϶���ļ�ֵ
     * @param [IN] dest {const std::string&} dest���϶���ļ�ֵ
     * @param [IN] member {const std::string&} ������Ҫ�ƶ���Ԫ��
     * @return {int} �����жϽ��, ����ֵ����:
     *    1: ������Ԫ�ر��ƶ�
     *    0: ������Ԫ�ز���src���ϳ�Ա����src������
     *   -1: �������src/dest���϶�������ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int smove(const std::string& src, const std::string& dest,
              const std::string& member);

    /**
     * @description
     *   ����Ĵ�key�����ļ����е���һ��Ԫ��, �����ص�����Ԫ��.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::string&} ��ŵ�����Ԫ��
     * @return {int} �����жϽ��, ����ֵ����:
     *    1: �������һ��Ԫ��
     *    0: key������
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int spop(const std::string& key, std::string& result);

    /**
     * @description
     *   ����Ĵ�key�����ļ����е���count��Ԫ��, �����ص�����Ԫ��.
     *   3.2���ϰ汾֧��.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::vector<std::string>&} ��ŵ�����Ԫ��
     * @param [IN] count {int} ����Ԫ�صĸ���, ȡֵ����:
     *   >0: ����count����Ԫ��, ���count���ڼ���Ԫ������, �򵯳�����Ԫ��
     *    0: ������, ���ؿ��б�
     * @return {int} �����жϽ��, ����ֵ����:
     *   >0: ����Ԫ�صĸ���
     *    0: key������
     *   -1: countΪ��ֵ��key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int spop(const std::string& key,
             std::vector<std::string>& result, int count);

    /**
     * @description
     *   ����Ĵ�key�����ļ����з���һ��Ԫ��. 
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::string&} ��ŷ��ص�Ԫ��
     * @return {int} �����жϽ��, ����ֵ����:
     *    1: �������һ��Ԫ��
     *    0: key������
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int srandmember(const std::string& key, std::string& result);

    /**
     * @description
     *   ����Ĵ�key�����ļ����з���count��Ԫ��. 
     * @param [IN] key {const std::string&} ���϶����key
     * @param [OUT] result {std::string&} ��ŷ��ص�Ԫ��
     * @param [IN] count {int} ����Ԫ�صĸ���, ȡֵ����:
     *   >0: ����count�����ظ���Ԫ��, ���count���ڼ���Ԫ������, �򷵻�����Ԫ��
     *    0: ���ؿ��б�
     *   <0: ����count��Ԫ��(���ظ�)
     * @return {int} �����жϽ��, ����ֵ����:
     *   >0: ����Ԫ�صĸ���
     *    0: key�����ڻ�count=0
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int srandmember(const std::string& key,
                    std::vector<std::string>& result, int count);

    /**
     * @description
     *   ��key�����ļ������Ƴ�ָ��Ԫ��. ��������в�����ָ��Ԫ��, �����. ���
     *   key������, �򵱳ɿռ���, ������0.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [IN] member {const std::vector<std::string>&} ��Ҫ�Ƴ���Ԫ���б�
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: �Ƴ���Ԫ�ظ���, �����������Ͳ����ڵ�Ԫ��
     *    0: key�����ڻ���û��Ԫ�ر��Ƴ�
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    long long srem(const std::string& key,
                   const std::vector<std::string>& member);

    /**
     * @description
     *   ���ڵ���key�����ļ����е�Ԫ��
     * @param [IN] key {const std::string&} ���϶����key
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
    int sscan(const std::string& key, int cursor,
              std::vector<std::string>& result,
              const char* pattern = NULL, int count = 10);

private:
    bool set_operation(const char* op, const std::vector<std::string>& keys,
                       std::vector<std::string>& result);
    long long set_operation_with_store(const char * op, const std::string& dest,
                                       const std::vector<std::string>& keys);
};

#endif /* __REDIS_SET_H__ */
