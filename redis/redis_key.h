#ifndef __REDIS_KEY_H__
#define __REDIS_KEY_H__

#include <stdint.h>
#include <vector>
#include <string>

#include "redis_command.h"

class SortParams;
class MigrateParams;

class redis_key : virtual public redis_command
{
public:
    redis_key(const std::string& host, uint16_t port);
    ~redis_key(){}

public:
    /**
     * ɾ��ָ����һ����һ��keys, ���Բ����ڵ�key
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ��keys
     * @return {int} ����ʵ��ɾ����key�ĸ���, ����:
     *   0: δɾ���κ�key
     *  >0: ʵ��ɾ����key�ĸ���, ��ֵ����С������key�ĸ���
     *  -1: ����
     */
    int del(const std::vector<std::string>& vkeys);
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
     * @param [IN] vkeys {const std::vector<std::string>&} ������key
     * @return {int} ���ش��ڵ�key�ĸ���, ����ֵ����:
     *   >0: ���ڵ�key�ĸ���, ���������ͬһ��key���ظ����, �򽫻ᱻ��μ���
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int exists(const std::vector<std::string>& vkeys);

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
     *   >=0: ����������key�ĸ���
     *    -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int keys(const std::string& pattern, std::vector<std::string>& result);

    /**
     * @description
     *   ��ָ�����ݴӵ�ǰ��redis-serverʵ��Ǩ�Ƶ�Ŀ��redis-serverʵ����ָ������
     *   ����. �ò�����һ��ԭ�Ӳ���, �������ɹ�ʱ, ����ȷ�����ݴ�ԭʵ��ɾ������
     *   ����Ŀ��ʵ����.
     *   ѡ��:
     *      COPY - ���ݲ���ԭʵ��ɾ��.
     *      REPLACE - ����Ŀ��ʵ�����Ѿ����ڵ�ͬ����ֵ
     *      KEYS - ���key����Ϊ���ַ���, ������һ�ο���Ǩ�ƶ��key, Ǩ�Ƶļ�ֵ
     *       ��keys����ָ��
     * @param [IN] host {const std::string&} Ŀ��ʵ���ķ�������ַ.
     * @param [IN] port {uint16_t} Ŀ��ʵ���ķ���˿�.
     * @param [IN] key {const std::string&} ��ҪǨ�������ݵļ�ֵ. ȡֵΪ "" ʱ,
     *   ��ʾʹ�� KEYS ѡ��.
     * @param [IN] dest_db {int} Ŀ��ʵ�������ݿ�ID��.
     * @param [IN] timeout {long long} Ǩ�ƹ��̵ĳ�ʱʱ��, ��λ: ����.
     * @param [IN] params {MigrateParams*} migrate ѡ��
     * @return {bool} Ǩ���Ƿ�ɹ�.
     * @author chen.tengfei
     * @date 2017-04-26
     */
    bool migrate(const std::string& host, uint16_t port, const std::string& key,
        int dest_db, long long timeout, MigrateParams* params = NULL);

    /**
     * @description
     *   ��ָ��key�ӵ�ǰ���ݿ��ƶ���ָ�����ݿ�. �������ֹ�ڼ�Ⱥģʽ��ʹ��.
     * @param [IN] key {const std::string&} ָ���ƶ��ļ�ֵ
     * @param [IN] db {int} Ŀ�����ݿ�
     * @return {int} �Ƿ��ƶ��ɹ�, ����ֵ����:
     *    1: Ǩ�Ƴɹ�
     *    0: δǨ�Ƴɹ�, Ŀ�����ݿ�����ͬ���ֵĸ���key,��key�������ڵ�ǰ���ݿ���
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-26
     */
    int move(const std::string& key, int db);

    /**
     * @description
     *   ����ָ��key������Value�����õĴ���. ��������Ҫ���ڵ���
     * @param [IN] key {const std::string&} ָ���ļ�ֵ
     * @return {int} �������ô���, ����ֵ����:
     *   >0: ���ô���
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-05
     */
    int object_refcount(const std::string& key);

    /**
     * @description
     *   ����ָ��key������Value���ڲ���ʾ��ʽ(��ѹ����ʽ).
     * @param [IN] key {const std::string&} ָ���ļ�ֵ
     * @param [OUT] encode {std::string&} ��ŷ��ؽ��(ѹ����ʽ), ��ֵ��ʾkey��
     *   ����.
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �����ɹ�
     *   false: ����ʧ��
     * @author chen.tengfei
     * @date 2017-05-05
     */
    bool object_encoding(const std::string& key, std::string& encode);

    /**
     * @description
     *   ����ָ��key������Value�Ŀ���ʱ��(û�ж�д����), ��λ: ��
     * @param [IN] key {const std::string&} ָ���ļ�ֵ
     * @return {long long} ���ؿ���ʱ��, ����ֵ����:
     *   >0: ����ʱ��
     *    0: key������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-05
     */
    long long object_idletime(const std::string& key);

    /**
     * @description
     *   �Ƴ�ָ��key�ĳ�ʱʱ��.
     * @param [IN] key {const std::string&} ָ���ļ�ֵ
     * @return {int} ���ز������, ����ֵ����:
     *    1: �ɹ��Ƴ�����ʱ��
     *    0: key�����ڻ���keyû����������ʱ��
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int persist(const std::string& key);

    /**
     * @description
     *   ����key������ʱ��. ����ʱ�䳬ʱ, key���ᱻɾ��
     * @param [IN] key {const std::string&} ������key
     * @param [IN] milliseconds {long long} ����ʱ��, ��λ: ����. ���ʱ��Ϊ����
     *   ��, key��������ɾ��
     * @return {int} ����ֵ����:
     *    1: ���óɹ�
     *    0: key�����ڻ�����ʱ���޷�������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int pexpire(const std::string& key, long long milliseconds);

    /**
     * @description
     *   ��UNIXʱ�������key������ʱ��.
     * @param [IN] key {const std::string&} ������key
     * @param [IN] timestamp {time_t} �Ժ���Ϊ��λ��UNIXʱ���. ���ʱ�����ʾ��
     *   ȥ��ʱ��, key��������ɾ��
     * @return {int} ����ֵ����:
     *    1: ���óɹ�
     *    0: key�����ڻ�ʱ����޷�������
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int pexpireat(const std::string& key,
                  unsigned long long milliseconds_timestamp);

    /**
     * @description
     *   ����ָ��keyʣ�������ʱ��, ��λ: ����
     * @param [IN] key {const std::string&} ������key
     * @return {long long} ����ʣ������ʱ��, ����ֵ����:
     *   >=0: ʣ������ʱ��
     *    -1: key���ڵ���û�й�������ʱ��
     *    -2: key������
     * @author chen.tengfei
     * @date 2017-05-09
     */
    long long pttl(const std::string& key);

    /**
     * @description
     *   �ӵ�ǰ���ݿⷵ��һ�������key.
     * @param [OUT] out {std::string&} �洢���ص�key
     * @return {int} ���ز������, ����ֵ����:
     *    1: �ɹ�����һ�������key
     *    0: ��ǰ���ݿ�Ϊ��
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int randomkey(std::string& out);

    /**
     * @description
     *   ��key������Ϊnew_key. ���key������, ��ʧ��; ���new_key����, ��ֵ����
     *   ����.
     * @param [IN] key {const std::string&} ������key
     * @param [IN] new_key {const std::string&} �µļ���
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �����ɹ�
     *   false: ����ʧ��
     * @author chen.tengfei
     * @date 2017-05-09
     */
    bool rename(const std::string& key, const std::string& new_key);

    /**
     * @description
     *   ��new_key������ʱ, ��key������Ϊnew_key. ���key������, ��ʧ��.
     * @param [IN] key {const std::string&} ������key
     * @param [IN] new_key {const std::string&} �µļ���
     * @return {int} ���ز������, ����ֵ����:
     *    1: key �ɹ�����Ϊ new_key
     *    0: new_key �Ѿ�����
     *   -1: key ������
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int renamenx(const std::string& key, const std::string& new_key);

    /**
     * @description
     *   �����л����������л�ֵ, ��������� key ��. ��� key �Ѿ�����, �򷵻�һ
     *   ��"Target key name is busy"�Ĵ���, ����ʹ�� REPLACE ģʽ.
     * @param [IN] key {const std::string&} ������key.
     * @param [IN] TTL {unsigned long long} key������ʱ��, ��λ: ����.
     *   ���ttlΪ0, ����������ʱ��.
     * @param [IN] serialized_value {const std::string&} ���л���ֵ.
     * @param [IN] is_replace {bool} �Ƿ�ʹ�� REPLACE ģʽ(redis 3.0�����ϰ汾).
     *    true: ʹ�� REPLACE ģʽ.
     *   false: ��ʹ�� REPLACE ģʽ.
     * @return {bool} ���ز������, ����ֵ����:
     *    true: �����ɹ�
     *   false: ����ʧ��
     * @author chen.tengfei
     * @date 2017-05-09
     */
    bool restore(const std::string& key, unsigned long long TTL,
                 const std::string& serialized_value, bool is_replace = false);

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

    /**
     * @description
     *   ��key������list, set, zset�е�Ԫ������, ��������洢��ָ����dest��.
     *   Ĭ�ϰ�����ֵ��������, �Ƚϵ�Ԫ�ر�����˫���ȸ���������.
     * @param [IN] key {const std::string&} ������key.
     * @param [IN] dest {const std::string&} �洢������б��ֵ.
     * @param [IN] param {SortParams*} �������
     * @return {int} ���ؽ���б�ĳ���, ����ֵ����:
     *  >=0: ����б�ĳ���
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int sort(const std::string& key, const std::string& dest,
             SortParams* params = NULL);

    /**
     * @description
     *   ����key������list, set, zset�е�������Ԫ��.Ĭ�ϰ�����ֵ��������, �Ƚ�
     *   ��Ԫ�ر�����˫���ȸ���������.
     * @param [IN] key {const std::string&} ������key.
     * @param [IN] result {std::vector<std::string>&} �洢������б�.
     * @param [IN] param {SortParams*} �������
     * @return {int} ���ؽ���б�ĳ���, ����ֵ����:
     *  >=0: ����б�ĳ���
     *   -1: ����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int sort(const std::string& key, std::vector<std::string>& result,
             SortParams* params = NULL);

    /**
     * @description
     *   �ı����key���������ʱ��, ���Բ����ڵ�key.
     * @param [IN] vkeys {const std::vector<std::string>&} ָ����һ��keys
     * @return {int} ���ر�touched��key�ĸ���, ����:
     *   0: û��key�յ�Ӱ��
     *  >0: ʵ���ܵ�Ӱ���key�ĸ���, ��ֵ����С������key�ĸ���
     *  -1: ����
     * @author chen.tengfei
     * @date 2017-05-10
     */
    int touch(const std::vector<std::string>& vkeys);
    int touch(const std::string& key);

    /**
     * @description
     *   ����ָ��key��ʣ������ʱ��, ��λ: ��
     * @param [IN] key {const std::string&} ������key
     * @return {long long} ����ʣ������ʱ��, ����ֵ����:
     *   >=0: ʣ������ʱ��
     *    -1: key���ڵ����Ѿ�����
     *    -2: key������
     * @author chen.tengfei
     * @date 2017-05-10
     */
    long long ttl(const std::string& key);

    /**
     * @description
     *   ����key���洢��value�����ݽṹ����, ���Է���string, list, set, zset ��
     *   hash�Ȳ�ͬ������.
     * @param [IN] key {const std::string&} ������key
     * @return {std::string} ���ش洢�����ݽṹ����, Ϊ�ձ�ʾkey������.
     * @author chen.tengfei
     * @date 2017-05-10
     */
    std::string type(const std::string& key);
    
};

/**
 * ���� sort �������
 */
class SortParams
{
public:
    std::vector<std::string>& getParams() {return params;}

    /**
     * @description
     *   ��Ҫһ��ģʽ, ����������������Ȩ�ص�key��.
     * @param [IN] pattern {const std::string&} ������������Ȩ�ص�key��ģʽ.
     * @return {SortParams&} ���� SortParams �����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    SortParams& by(const std::string& pattern);

    /**
     * @description
     *   ʹsort���������������. ��get���ÿ��Ի�ȡδ��������ⲿkey.
     * @return {SortParams&} ���� SortParams �����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    SortParams& nosort();

    /**
     * @description
     *   ��Ҫһ��ģʽ, ����������������key��
     * @param [IN] pattern {const std::string&} ����������������key����ģʽ.
     * @return {SortParams&} ���� SortParams �����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    SortParams& get(const std::string& pattern);

    /**
     * @description
     *   ���Ʒ���������������.
     * @param [IN] offset {int} ����0, ���������±���ʼֵ.
     * @param [IN] count {int} �ӽ����offsetָ����λ�ÿ�ʼѡȡ��Ԫ������
     * @return {SortParams&} ���� SortParams �����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    SortParams& limit(int offset, int count);

    /**
     * @description
     *   �������������, ��ѡ��ΪĬ��ѡ��.
     * @return {SortParams&} ���� SortParams �����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    SortParams& asc();

    /**
     * @description
     *   �������������
     * @return {SortParams&} ���� SortParams �����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    SortParams& desc();

    /**
     * @description
     *   ���ֵ�������.
     * @return {SortParams&} ���� SortParams �����
     * @author chen.tengfei
     * @date 2017-05-09
     */
    SortParams& alpha();

private:
    std::vector<std::string> params;
};

/**
 * ���� migrate �������
 */
class MigrateParams
{
public:
    std::vector<std::string>& getParams() {return params;}

    /**
     * @description
     *   ��������, ���ݲ��ӱ���ʵ��ɾ��.
     * @return {MigrateParams&} ���� MigrateParams �����
     * @author chen.tengfei
     * @date 2017-06-04
     */
    MigrateParams& copy();

    /**
     * @description
     *   ����Ŀ��ʵ�����Ѿ����ڵ�ͬ����ֵ.
     * @return {MigrateParams&} ���� MigrateParams �����
     * @author chen.tengfei
     * @date 2017-06-04
     */
    MigrateParams& replace();

    /**
     * @description
     *   ���key����Ϊ���ַ���, ������һ�ο���Ǩ�ƶ��key, Ǩ�Ƶļ�ֵ��keys����
     *   ָ��.
     * @param [IN] vkeys {const std::vector<std::string>&} ��ҪǨ�ƵĶ��key
     * @return {MigrateParams&} ���� MigrateParams �����
     * @author chen.tengfei
     * @date 2017-06-04
     */
    MigrateParams& keys(const std::vector<std::string>& vkeys);

private:
    std::vector<std::string> params;
};

#endif /* __REDIS_KEY_H__ */
