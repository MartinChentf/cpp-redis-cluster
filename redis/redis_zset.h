#ifndef __REDIS_ZSET_H__
#define __REDIS_ZSET_H__

#include "redis_commamd.h"

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
     *   >0: �������һ��Ԫ��
     *    0: key������
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
                      const char* nx_or_xx = NULL);

    /**
     * @description
     *   ��key���������򼯺���ָ����Ԫ��member�ķ���ֵ�����ۼ�. ��Ϊ��zincrby��
     *   ��һ��
     * @param [IN] key {const std::string&} ���򼯺϶����key
     * @param [IN/OUT] score {double} ����: �ۼӵķ���, ���: �ۼӺ�ķ���
     * @param [IN] member {const std::string&} ��Ҫ�ۼӵĳ�Ա
     * @return {int} ���ز������, ����ֵ����:
     *    true: �ۼӳɹ�
     *   false: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    bool zadd_incr(const std::string& key, double& score,
                        const std::string& member);

    long long zcard(const std::string& key);
    long long zcount(const std::string& key, double min, double max);
    long long zcount(const std::string& key, std::string min, std::string max);
    double zincrby(const std::string& key, double increment, const std::string& member);
};

#endif /* __REDIS_ZSET_H__ */
