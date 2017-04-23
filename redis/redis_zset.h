#ifndef __REDIS_ZSET_H__
#define __REDIS_ZSET_H__

#include <string>
#include <vector>

#include "redis_command.h"

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
     * @param [IN] member_score
     *   {const std::vector<std::pair<std::string, double>>&} Ԫ��/������
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
            const std::vector< std::pair<std::string, double> >& member_score,
            int nx_or_xx = 0, bool is_change = false);

    long long zadd_nx(const std::string& key,
            const std::vector< std::pair<std::string, double> >& member_score,
            bool is_change = false);

    long long zadd_xx(const std::string& key,
            const std::vector< std::pair<std::string, double> >& member_score,
            bool is_change = false);

    long long zadd_ch(const std::string& key,
            const std::vector< std::pair<std::string, double> >& member_score,
            int nx_or_xx = 0);

    /**
     * @description
     *   ��key���������򼯺���ָ����Ԫ��member�ķ���ֵ�����ۼ�. ��Ϊ��zincrby��
     *   ��һ��.
     * @param [IN] key {const std::string&} ���򼯺϶����key
     * @param [IN] score {double} �ۼӵķ���
     * @param [IN] member {const std::string&} ��Ҫ�ۼӵĳ�Ա
     * @param [OUT] result {std::string&} �洢�ۼӽ��
     * @return {bool} ���ز������, ����ֵ����:
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
     *   ����key���������򼯺���, ����ֵ��[min, max](������)��Χ�ڵ�Ԫ�ظ���.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {double} ������������.
     * @pzrzm [IN] max {double} ������������.
     * @return {long long} ����ָ��������Χ��Ԫ�ظ���, ����ֵ����:
     *   >=0: ָ��������Χ��Ԫ�ظ���
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-05
     */
    long long zcount(const std::string& key, double min, double max);

    /**
     * @description
     *   ����key���������򼯺���, ����ֵ��[min, max](������)��Χ�ڵ�Ԫ�ظ���.
     *   1) min��max����ʹ��-inf��+inf. ���������ڲ�֪�����򼯺���ͺ���߷���ֵ
     *      �������ʹ�ñ�����.
     *   2) Ĭ�������, ���������ȡֵʹ�ñ�����, �����ڷ���ֵǰ�����'('������
     *      ʹ�ÿ�����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ������������.
     * @pzrzm [IN] max {const std::string&} ������������.
     * @return {long long} ����ָ��������Χ��Ԫ�ظ���, ����ֵ����:
     *   >=0: ָ��������Χ��Ԫ�ظ���
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @param [IN/OUT] name {type} 
     * @return {type} 
     * @author chen.tengfei
     * @date 2017-04-15
     */
    long long zcount(const std::string& key, const std::string& min,
                     const std::string& max);

    /**
     * @description
     *   ��key���������򼯺���ָ����Ԫ��member�ķ���ֵ�ۼ�increment.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] increment {double} �ۼ�ֵ
     * @param [IN] member {const std::string&} ��Ҫ�ۼӵĳ�Ա
     * @param [OUT] result {std::string*} ��Ϊ��ʱ, �洢�ۼӽ��
     * @return {int} ���ز������, ����ֵ����:
     *    true: �ۼӳɹ�
     *   false: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-05
     */
    bool zincrby(const std::string& key, double increment,
                    const std::string& member, std::string* result = NULL);

    /**
     * @description
     *   �������numkeys�����򼯺ϵĽ���, ��������������dest��.
     * @param [IN] dest {const std::string&} ��ż����������򼯺϶���
     * @param [IN] keys {const std::vector<std::string>&}
     *   ������һ�����򼯺�/����(���м��ϳ�Ա�ķ�ֵ������Ϊ1).
     * @param [IN] weights {const std::vector<double>*} ���򼯺϶�Ӧ��Ȩ���б�.
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
    long long zinterstore(const std::string& dest,
                          const std::vector<std::string>& keys,
                          const std::vector<double>* weights = NULL,
                          const char* aggregate = "SUM");

    /**
     * @description
     *   �������numkeys�����򼯺ϵĽ���, ��������������dest��.
     * @author chen.tengfei
     * @date 2017-04-06
     */
    long long zunionstore(const std::string& dest,
                          const std::vector<std::string>& keys,
                          const std::vector<double>* weights = NULL,
                          const char* aggregate = "SUM");

    /**
     * @description
     *   �����򼯺ϵ����г�Ա������ͬ����ֵʱ, ���򼯺ϵ�Ԫ�ػ���ݳ�Ա���ֵ���
	 *   (lexicographical ordering)����������, �������������Է���key����������
	 *   ������, ֵ����Ԫ��min��Ԫ��max֮��ĳ�Ա����. min��max���÷��ο�
	 *   zrangebylex����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ����Ԫ����Сֵ
     * @param [IN] max {const std::string&} ����Ԫ�����ֵ
     * @return {long long} ����ָ�������ڵĳ�Ա����, ����ֵ����:
     *   >=0: ָ�������ڵĳ�Ա����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    long long zlexcount(const std::string& key, const std::string& min,
                        const std::string& max);

    /**
     * @description
     *   ����key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ��. ���г�
     *   Ա���շ���ֵ��������, ������ͬ��Ԫ�ذ����ֵ�����������. start��stop�ǻ�
     *   ��0���±�, ��Ϊ����, -1��ʾ���һ��Ԫ��, -2��ʾ�����ڶ���Ԫ��. ���
     *   start�����б�ĩβ�±����start > stop, ���ؽ����Ϊ���б�. ���stop����
     *   �б��±�, ��ᱻ�����б�ĩβ�±�.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] stop {int} �����±�
     * @param [OUT] result {std::vector<std::string>&} �洢���صĽ����
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrange(const std::string& key, int start, int stop,
               std::vector<std::string>& result);

    /**
     * @description
     *   ����key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ�ؼ������.
     *   start/stopȡֵ��Χ�ο�zrange����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] stop {int} �����±�
     * @param [OUT] result {std::vector<std::pair<std::string, double>>&}
     *   �洢���صĽ����
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrange_with_scores(const std::string& key, int start, int stop,
        std::vector< std::pair<std::string, double> >& result);

    /**
     * @description
     *   �����򼯺ϵ����г�Ա������ͬ����ֵʱ, ���򼯺ϵ�Ԫ�ػ���ݳ�Ա���ֵ���
	 *   (lexicographical ordering)����������, �������������Է���key����������
	 *   ������, ֵ����Ԫ��min��Ԫ��max֮��ĳ�Ա.
	 *   1) ������򼯺���Ԫ�صķ�����һ��, �򷵻ؽ��Ԫ�ؽ���׼ȷ.
	 *   2) �ַ������ն������ֽ�������бȽ�, ��С��������. ����ַ�����ʹ���˷�
	 *      ASCII�ַ���(����UTF-8), ���Ӱ�췵�ؽ��
	 *   3) ��Ч��min��max������'('��'['��ͷ, ��ʾ�����򲻰���min/maxԪ��.
	 *   4) min��maxλ�ÿ���ʹ��'-'��'+'��ʾ��Сֵ�����ֵ.
	 *      e.g. zrangebylex("foo", "-", "+", out)
	 *           ���᷵�����򼯺�foo�����е�Ԫ��
	 *   5) max��min��λ�ò��ܽ���, ���򽫻᷵�ؿռ�
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ����Ԫ����Сֵ
     * @param [IN] max {const std::string&} ����Ԫ�����ֵ
     * @param [OUT] result {std::vector<std::string>&} �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ, ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebylex(const std::string& key, const std::string& min,
                    const std::string& max, std::vector<std::string>& result,
                    int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {double} ������������.
     * @pzrzm [IN] max {double} ������������.
     * @param [OUT] result {std::vector<std::string>&} �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ, ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore(const std::string& key, double min, double max,
                      std::vector<std::string>& result,
                      int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��
     *   1) min��max����ʹ��-inf��+inf. ���������ڲ�֪�����򼯺���ͺ���߷���ֵ
     *      �������ʹ�ñ�����.
     *   2) Ĭ�������, ���������ȡֵʹ�ñ�����, �����ڷ���ֵǰ�����'('������
     *      ʹ�ÿ�����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ������������.
     * @pzrzm [IN] max {const std::string&} ������������.
     * @param [OUT] result {std::vector<std::string>&} �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ(����0), ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore(const std::string& key, const std::string& min,
                      const std::string& max, std::vector<std::string>& result,
                      int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ�ؼ���
     *   ����
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {double} ������������.
     * @pzrzm [IN] max {double} ������������.
     * @param [OUT] result {std::vector<std::pair<std::string, double>>&}
     *   �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ, ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore_with_scores(const std::string& key,
        double min, double max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ�ؼ���
     *   ����
     *   1) min��max����ʹ��-inf��+inf. ���������ڲ�֪�����򼯺���ͺ���߷���ֵ
     *      �������ʹ�ñ�����.
     *   2) Ĭ�������, ���������ȡֵʹ�ñ�����, �����ڷ���ֵǰ�����'('������
     *      ʹ�ÿ�����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ������������.
     * @pzrzm [IN] max {const std::string&} ������������.
     * @param [OUT] result {std::vector<std::pair<std::string, double>>&}
     *   �洢�����
     * @param [IN] offset {int} �ӽ������ѡȡ���±���ʼֵ, ȡֵ��Χ:
     *   >=0: ������е��±�ֵ
     *    -1: ���������Ƭ, Ĭ��ֵ
     * @paran [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     *   >0: Ϊ��Чֵ
     *    0: Ĭ��ֵ, ��offset < 0ʱ, ����offset��count��������
     * @return {int} ���ؽ������С, ����ֵ����:
     *   >=0: �������С
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore_with_scores(const std::string& key,
        const std::string& min, const std::string& max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����Ԫ��member��key���������򼯺��е�����, ���򼯺ϰ��շ���ֵ��������.
     *   ������(���±�)�ǻ���0��, Ҳ����˵, scoreֵ��С�ĳ�Ա����Ϊ0.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] member {const std::string&} ָ����Ԫ��member.
     * @return {int} ����ָ��Ԫ�������򼯺��е�����, ����ֵ����:
     *   >=0: ָ��Ԫ�������򼯺��е�����
     *    -1: 1) �����key��value���ʹ���(non-sortedset)
     *        2) key�����ڻ���member������
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrank(const std::string& key, const std::string& member);

    /**
     * @description
     *   ��key���������򼯺����Ƴ�ָ��Ԫ��. ��������в�����ָ��Ԫ��, �����. ��
     *   ��key������, �򵱳ɿռ���, ������0.
     * @param [IN] key {const std::string&} ���϶����key
     * @param [IN] member {const std::vector<std::string>&} ��Ҫ�Ƴ���Ԫ���б�
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: �Ƴ���Ԫ�ظ���, �����������Ͳ����ڵ�Ԫ��
     *    0: key�����ڻ���û��Ԫ�ر��Ƴ�
     *   -1: �����key��value���ʹ���(non-set)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    long long zrem(const std::string& key,
                   const std::vector<std::string>& member);

    /**
     * @description
     *   �����򼯺ϵ����г�Ա������ͬ����ֵʱ, ���򼯺ϵ�Ԫ�ػ���ݳ�Ա���ֵ���
	 *   (lexicographical ordering)����������, ���������������Ƴ�key����������
	 *   ������, ֵ����Ԫ��min��Ԫ��max֮��ĳ�Ա.
	 *   min��max�ĺ�����ο�zrangebylex����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ����Ԫ����Сֵ
     * @param [IN] max {const std::string&} ����Ԫ�����ֵ
     * @return {int} �����Ƴ�Ԫ�ص�����, ����ֵ����:
     *   >=0: �Ƴ�Ԫ�ص�����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebylex(const std::string& key, const std::string& min,
                       const std::string& max);

    /**
     * @description
     *   �Ƴ�key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ��.
     *   start��stop�ĺ�����ο�zrange����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] stop {int} �����±�
     * @return {int} �����Ƴ�Ԫ�ص�����, ����ֵ����:
     *   >=0: �Ƴ�Ԫ�ص�����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebyrank(const std::string& key, int start, int stop);

    /**
     * @description
     *   �Ƴ�key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {double} ������������.
     * @pzrzm [IN] max {double} ������������.
     * @return {int} �����Ƴ�Ԫ�ص�����, ����ֵ����:
     *   >=0: �Ƴ�Ԫ�ص�����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebyscore(const std::string& key, double min, double max);

    /**
     * @description
     *   �Ƴ�key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��
     *   1) min��max����ʹ��-inf��+inf. ���������ڲ�֪�����򼯺���ͺ���߷���ֵ
     *      �������ʹ�ñ�����.
     *   2) Ĭ�������, ���������ȡֵʹ�ñ�����, �����ڷ���ֵǰ�����'('������
     *      ʹ�ÿ�����.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] min {const std::string&} ������������.
     * @pzrzm [IN] max {const std::string&} ������������.
     * @return {int} �����Ƴ�Ԫ�ص�����, ����ֵ����:
     *   >=0: �Ƴ�Ԫ�ص�����
     *    -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebyscore(const std::string& key, const std::string& min,
                         const std::string& max);

    /**
     * @description
     *   ����key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ��. ��������
     *   ֮��, �����ͷ���ֵͬzrange����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrange(const std::string& key, int start, int stop,
                  std::vector<std::string>& result);

    /**
     * @description
     *   ����key���������򼯺���[start, stop](������)ָ����Χ�ڵ�Ԫ�ؼ������.
     *   ��������֮��, �����ͷ���ֵͬzrange_with_scores����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrange_with_scores(const std::string& key,
        int start, int stop,
        std::vector< std::pair<std::string, double> >& result);

    /**
     * @description
     *   �����򼯺ϵ����г�Ա������ͬ����ֵʱ, ���򼯺ϵ�Ԫ�ػ���ݳ�Ա���ֵ���
	 *   (lexicographical ordering)����������, �������������Է���key����������
	 *   ������, ֵ����Ԫ��min��Ԫ��max֮��ĳ�Ա.
     *   ��������֮��, �����ͷ���ֵͬzrangebylex����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebylex(const std::string& key, const std::string& min,
        const std::string& max, std::vector<std::string>& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��.
     *   ��������֮��, �����ͷ���ֵͬzrangebyscore����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore(const std::string& key, double min, double max,
        std::vector<std::string>& result, int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ��.
     *   ��������֮��, �����ͷ���ֵͬzrangebyscore����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore(const std::string& key, const std::string& min,
        const std::string& max, std::vector<std::string>& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ�ؼ����
     *   ��. ��������֮��, �����ͷ���ֵͬzrangebyscore_with_scores����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore_with_scores(const std::string& key,
        double min, double max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����key���������򼯺��з���ֵ��[min, max](������)��Χ�ڵ�����Ԫ�ؼ����
     *   ��. ��������֮��, �����ͷ���ֵͬzrangebyscore_with_scores����һ��.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore_with_scores(const std::string& key,
        const std::string& min, const std::string& max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   ����Ԫ��member��key���������򼯺��е�����, ���򼯺ϰ��շ���ֵ�ݼ�����.
     *   ������(���±�)�ǻ���0��, Ҳ����˵, scoreֵ���ĳ�Ա����Ϊ0.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] member {const std::string&} ָ����Ԫ��member.
     * @return {int} ����ָ��Ԫ�������򼯺��е�����, ����ֵ����:
     *   >=0: ָ��Ԫ�������򼯺��е�����
     *    -1: 1) �����key��value���ʹ���(non-sortedset)
     *        2) key�����ڻ���member������
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrank(const std::string& key, const std::string& member);

    /**
     * @description
     *   ���ڵ���key���������򼯺��еļ�ֵ��
     * @param [IN] key {const std::string&} ���򼯺϶����key
     * @param [IN] cursor {int} �α�ֵ, ��һ�ε���ʹ��0��Ϊ�α�.
     * @param [OUT] result {std::vector< std::pair<std::string, double> >&}
     *   �洢�����, �ڲ���׷�ӷ�ʽ�����α��������ӽ��ö�����, Ϊ��ֹ���ܽ��
     *   �������¸��������,�û����ڵ��ñ�����ǰ������ö���.
     * @param [IN] pattern {const char*} glob����ģʽ����, �ǿ�ʱ��Ч
     * @param [IN] count {int} �޶������Ԫ�ص�����, Ĭ��ֵ:10
     * @return {int} ��һ���α�λ��, ����ֵ����:
     *   >0: ��һ���α�λ��
     *    0: ��������
     *   -1: key��value���ʹ���(non-hash)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zscan(const std::string& key, int cursor,
              std::vector< std::pair<std::string, double> >& result,
              const char* pattern = NULL, int count = 10);

    /**
     * @description
     *   ����key���������򼯺���Ԫ��member�ķ���.
     * @param [IN] key {const std::string&} ���򼯺϶����key.
     * @param [IN] member {const std::string&} ָ����Ԫ��member.
     * @param [OUT] soube {std::string&} �洢ָ��Ԫ�صķ���
     * @return {int} ���ز������, ����ֵ����:
     *    1: �ɹ�����ָ��Ԫ�صķ���
     *    0: key�����ڻ���member������
     *   -1: �����key��value���ʹ���(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zscore(const std::string& key, const std::string& member,
               std::string& score);

private:
    long long zstore(const char* cmd, const std::string& dest,
                     const std::vector<std::string>& keys,
                     const std::vector<double>* weights,
                     const char* aggregate);

    int zrangeby(const char* cmd, const std::string& key,
                 const std::string& min, const std::string& max,
                 std::vector<std::string>& result,
                 int offset, int count, bool with_scores = false);

    // ��Ԫ��/������ɵ��б�ת����Ԫ��/������ֵ�����, �����ؼ�ֵ�Ը���
    // ����б���Ԫ�ز��ǳɶԳ���, �򷵻�-1
    int parse_element_score(const std::vector<std::string>& in,
                            std::vector< std::pair<std::string, double> >& out);
};

#endif /* __REDIS_ZSET_H__ */
