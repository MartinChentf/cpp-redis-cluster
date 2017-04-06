#ifndef __REDIS_ZSET_H__
#define __REDIS_ZSET_H__

#include <string>
#include <map>

#include "redis_command.h"

class redis_client;

class redis_zset : public redis_command
{
public:
    redis_zset(redis_client* client):redis_command(client){}
    ~redis_zset(){}

public:
    /**
     * @description
     *   ��key���������򼯺����������ָ����Ԫ��. ÿ��Ԫ�ض�Ӧһ������.
     *   1) ��������ĳ�Ա�Ѿ����������򼯺���, ��������������������λ��
     *   2) ���key������, ���½�һ�����򼯺ϲ�������/��Ա��ӵ����򼯺���
     *   3) ���key��value���ʹ���(non-sortedset), �򷵻�һ������
     *   4) ����֧��˫����64λ������
     *   5) ���򼯺ϰ��շ�����������, �������Ԫ�ط���, ����Ԫ�ص��ֵ�˳������
     * @param [IN] key {const std::string&} ���򼯺϶����key
     * @param [IN] score_mems {const std::map<double, std::string>&} ����/Ԫ�ض�
     * @param [IN] nx_or_xx {int} �Ƿ�ʹ��NX��XX��־, Ĭ��ֵΪ0, ȡֵ��Χ����:
     *    1: ʹ��XX��־, ֻ�����Ѿ����������򼯺��е�Ԫ�صķ���, ��������Ԫ��
     *    0: ����NX/XX��־
     *   -1: ʹ��NX��־, �������Ѿ����������򼯺��е�Ԫ�صķ���, ֻ������Ԫ��
     * @param [IN] is_change {bool} �Ƿ��޸ķ���ֵ, Ĭ��ֵΪfalse, ȡֵ����:
     *    true: ����ֵΪ���б��ı��Ԫ�صĸ���, �����¼�Ԫ�غͷ������µ�Ԫ��
     *   false: ����ֵΪ�����ӵ�Ԫ�ظ���
     * @return {int} ���ز������, ����ֵ����:
     *   >=0: �����ӵ�Ԫ�ظ���
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    long long zadd(const std::string& key,
                   const std::map<double, std::string>& score_mems,
                   int nx_or_xx = 0, bool is_change = false);

    long long zadd_nx(const std::string& key,
                      const std::map<double, std::string>& score_mems,
                      bool is_change = false);

    long long zadd_xx(const std::string& key,
                      const std::map<double, std::string>& score_mems,
                      bool is_change = false);

    long long zadd_ch(const std::string& key,
                      const std::map<double, std::string>& score_mems,
                      int nx_or_xx = 0);

    /**
     * @description
     *   ��key���������򼯺���ָ����Ԫ��member�ķ���ֵ�����ۼ�. ��Ϊ��zincrby��
     *   ��һ��.
     * @param [IN] key {const std::string&} ���򼯺϶����key
     * @param [IN] score {double} ����: �ۼӵķ���, ���: �ۼӺ�ķ���
     * @param [IN] member {const std::string&} ��Ҫ�ۼӵĳ�Ա
     * @param [OUT] result {std::string&} �洢�ۼӽ��
     * @return {int} ���ز������, ����ֵ����:
     *    true: �ۼӳɹ�
     *   false: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    bool zadd_incr(const std::string& key, double score,
                   const std::string& member, std::string& result);

    /**
     * @description
     *   ����key���������򼯺ϵĻ���(���ϵ�Ԫ�ظ���).
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ���ϵĻ���.
     *    0: key������.
     *   -1: �����key��value���ʹ���(non-set).
     * @author chen.tengfei
     * @date 2017-04-05
     */
    long long zcard(const std::string& key);

    /**
     * @description
     *   ����key���������򼯺���, ����ֵ��[min, max]���䷶Χ�ڵĳ�Ա.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {double} ������������.
     * @pzrzm [IN] max {double} ������������.
     * @return {long long} ����ָ��������Χ��Ԫ�ظ���.
     * @author chen.tengfei
     * @date 2017-04-05
     */
    long long zcount(const std::string& key, double min, double max);

    /**
     * @description
     *   ��key���������򼯺���ָ����Ԫ��member�ķ���ֵ�ۼ�increment.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] increment {double} �ۼ�ֵ
     * @param [IN] member {const std::string&} ��Ҫ�ۼӵĳ�Ա
     * @param [OUT] result {std::string&} �洢�ۼӽ��
     * @return {int} ���ز������, ����ֵ����:
     *    true: �ۼӳɹ�
     *   false: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-05
     */
    bool zincrby(const std::string& key, double increment,
                    const std::string& member, std::string& result);

    /**
     * @description
     *   �������numkeys�����򼯺ϵĽ���, ��������������dest��.
     * @param [IN] dest {const std::string&} ��ż����������򼯺϶���
     * @param [IN] numkeys {int} ��Ҫ��������򼯺ϵĸ���
     * @param [IN] keys {const std::vector<std::string>&}
     *   ������һ�����򼯺�. keys��������numkeysָ��, �������
     * @param [IN] weights {const std::vector<double>&} ���򼯺϶�Ӧ��Ȩ���б�.
     *   ÿһ�����϶�Ӧһ��Ȩ��ֵ. ��Ȩ���б�Ϊ��ʱ, ��ʾĬ��Ȩ��ֵΪ1.
     * @param [IN] aggregate {const char*} ָ��������ľۺϷ�ʽ. Ĭ��ֵΪ"SUM",
     *   ȡֵ����:
     *   "SUM": �����м�����ĳ����Ա��scoreֵ֮����Ϊ������иó�Ա��scoreֵ
     *   "MIN": �����м�����ĳ����Ա��scoreֵ����Сֵ��Ϊ������иó�Ա��scoreֵ
     *   "MAX": �����м�����ĳ����Ա��scoreֵ�����ֵ��Ϊ������иó�Ա��scoreֵ
     * @return {long long} �����dest��Ԫ�صĸ���, ȡֵ����:
     *   >=0: �����dest��Ԫ�صĸ���
     *    -1: �����dest/keys��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-06
     */
    long long zinterstore(const std::string& dest, int numkeys,
                          const std::vector<std::string>& keys,
                          const std::vector<double>& weights,
                          const char* aggregate = "SUM");

    /**
     * @description
     *   �������numkeys�����򼯺ϵĽ���, ��������������dest��.
     * @author chen.tengfei
     * @date 2017-04-06
     */
    long long zunionstore(const std::string& dest, int numkeys,
                          const std::vector<std::string>& keys,
                          const std::vector<double>& weights,
                          const char* aggregate = "SUM");

private:
    long long zstore(const char* cmd, const std::string& dest, int numkeys,
                     const std::vector<std::string>& keys,
                     const std::vector<double>& weights,
                     const char* aggregate);
};

#endif /* __REDIS_ZSET_H__ */
