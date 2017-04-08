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
    /**
     * @description
     *   blpop����������ʽ�б�ĵ���ԭ��. ����lpop�����������汾.
     *   1) �������������б�û���κ�Ԫ�ص�ʱ��, ���ӽ���blpop��������, ֱ�����
     *      �ͻ�������Щ�б��е�����һ���б�pushԪ�ػ��߳�ʱ. ��timeoutΪ0ʱ, ��
     *      ʾ����������
     *   2) ����������б���������һ���ǿ��б�, �򵯳���һ���ǿ��б��ͷ��Ԫ��.
     * @param [IN] keys {const std::vector<std::string>&} ��������Ԫ�ص��б���
     * @param [IN] timeout {long long} ��ʱʱ��
     * @param [OUT] result {std::map<std::string, std::string>&}
     *   �洢����Ԫ�ص�key�Լ���Ӧ��Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: ��ʱ
     *   -1: 1) src��dest��value���ʹ���(non-list)
     *       2) ��������
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int blpop(const std::vector<std::string>& keys, long long timeout,
              std::map<std::string, std::string>& result);

    /**
     * @description
     *   brpop����������ʽ�б�ĵ���ԭ��. ����rpop�����������汾. ������Ϊ��
     *   blpop��������, ����brpop�����Ǵ��б�β������Ԫ��.
     * @param [IN] keys {const std::vector<std::string>&} ��������Ԫ�ص��б���
     * @param [IN] timeout {long long} ��ʱʱ��
     * @param [OUT] result {std::map<std::string, std::string>&}
     *   �洢����Ԫ�ص�key�Լ���Ӧ��Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: ��ʱ
     *   -1: 1) src��dest��value���ʹ���(non-list)
     *       2) ��������
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int brpop(const std::vector<std::string>& keys, long long timeout,
              std::map<std::string, std::string>& result);
    /**
     * @description
     *   brpoplpush������rpoplpush�����������汾. ��src����Ԫ��ʱ, ��Ϊ��
     *   rpoplpush����һ��. ��srcΪ��ʱ, redis��������������, ֱ����Ŀͻ�����
     *   src��pushԪ�ػ��߳�ʱ. 
     * @param [IN] src {const std::string&} Դ�б����ļ�ֵ
     * @param [IN] dest {const std::string&} Ŀ���б����ļ�ֵ
     * @param [IN] timeout {long long} ��ʱʱ��
     * @param [OUT] result {std::string&} �洢������Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: ��ʱ
     *   -1: 1) src��dest��value���ʹ���(non-list)
     *       2) ��������
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int brpoplpush(const std::string& src, const std::string& dest,
                   long long timeout, std::string& result);
    /**
     * @description
     *   ����key�������б����±�Ϊindex��Ԫ��. indexΪ����0���±�, ��Ϊ����, 
     *   -1��ʾ���һ��Ԫ��, -2��ʾ�����ڶ���Ԫ��.
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
     * @return {int} ���ز������, ����ֵ����:
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
     *   ����key�������б���[start, end](������)��Χ�ڵ�Ԫ��. start��endΪ����0
     *   ���±�, ��Ϊ����, -1��ʾ���һ��Ԫ��, -2��ʾ�����ڶ���Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] end {int} �����±�
     * @param [OUT] result {std::vector<std::string>&} �洢ָ�������Ԫ�ؼ���
     * @return {bool} �����Ƿ�ɹ�, false��ʾ�����key��value���ʹ���(non-list)
     */
    bool lrange(const std::string& key, int start, int stop,
                std::vector<std::string>& result);

    /**
     * @description
     *   �Ƴ�key�������б���ǰcount��ֵ����value��Ԫ��.
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] count {int} �Ƴ�Ԫ�صĸ���. count��ȡֵ����:
     *   count > 0: ���б�ͷ����ʼ�Ƴ�ֵ����valueԪ��
     *   count < 0: ���б�β������ʼ�Ƴ�ֵ����valueԪ��
     *   count = 0: �Ƴ��б������е���valueԪ��
     * @param [IN] value {const std::string&} ��Ҫ�Ƴ���Ԫ��
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: �Ƴ�Ԫ�صĸ���
     *    0: key�����ڻ�value������
     *   -1: �����key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    long long lrem(const std::string& key, int count, const std::string& value);

    /**
     * @description
     *   ����key�������б����±�index����ֵΪvalue. indexȡֵ��ο�lindex����.
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] index {int} Ԫ���±�
     * @param [IN] value {const std::string&} �µ�Ԫ��
     * @return {bool} ���ز������, ����ֵ����:
     *    true: ������ֵ�ɹ�
     *   false: 1) key��value���ʹ���(non-list)
     *          2) indexԽ��
     * @author chen.tengfei
     * @date 2017-03-29
     */
    bool lset(const std::string& key, int index, const std::string& value);

    /**
     * @description
     *   �޼�key�������б�ʹ��ֻ����[start, stop]ָ����Χ�ڵ�Ԫ��. ���start����
     *   �б�ĩβ�±����start > stop, �����Ϊ���б�. �⽫���key��ɾ��. ���
     *   stop�����б��±�, ��ᱻ�����б�ĩβ�±�
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] start {int} ��ʼ�±�
     * @param [IN] stop {int} �����±�
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �����ɹ�
     *   false: key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    bool ltrim(const std::string& key, int start, int stop);

    /**
     * @description
     *   ����������key�������б��е����һ��Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [OUT] result {std::string&} �洢������Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �ɹ������б���Ԫ��
     *    0: key������
     *   -1: key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int rpop(const std::string& key, std::string& result);

    /**
     * @description
     *   ����������src�б��е����һ��Ԫ��, ���������dest�б��ͷ��. �ò�����ԭ
     *   �Ӳ���. ���src������, �򷵻ؿ��Ҳ����κβ���. ���src��dest��ͬ, ��
     *   ���б�ĩβԪ�ز�����Ԫ���Ƶ��б�ͷ��.
     * @param [IN] src {const std::string&} �б�����src
     * @param [IN] dest {const std::string&} �б�����dest
     * @param [OUT] result {std::string&} �洢������Ԫ��
     * @return {int} ���ز������, ����ֵ����:
     *    1: �����ɹ�
     *    0: src������
     *   -1: 1) src��dest��value���ʹ���(non-list)
     *       2) ��������
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int rpoplpush(const std::string& src, const std::string& dest,
                  std::string& result);

    /**
     * @description
     *   ��key�������б�β�����β���values�����е�Ԫ��. ���key������, ����Ԫ��
     *   ǰ�ȴ���һ�����б�
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] values {const std::vector<std::string>&} ��Ԫ�ؼ���
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *   -1: �����key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    long long rpush(const std::string& key,
                    const std::vector<std::string>& values);

    /**
     * @description
     *   ���key���ڲ������б����, ����key�������б�β������Ԫ��value. ���key
     *   ������, �򲻲���Ԫ��
     * @param [IN] key {const std::string&} �б�����key
     * @param [IN] value {const std::string&} ��Ԫ��
     * @return {long long} ���ز������, ����ֵ����:
     *   >0: ������Ԫ�غ�, ��ǰ�б�Ԫ�ظ���
     *   -1: �����key��value���ʹ���(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    long long rpushx(const std::string& key, const std::string& value);
    
};

#endif /* __REIDS_LIST_H__ */
