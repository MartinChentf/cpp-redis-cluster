#ifndef __REDIS_STRING_H__
#define __REDIS_STRING_H__

#include <string>
#include <vector>
#include <map>

#include "redis_command.h"

class redis_client;

class redis_string : public redis_command
{
public:
    enum BITOP {
        AND,
        OR,
        NOT,
        XOR,

        SIZE_BITOP
    };
    static const char* BITOP_STR[SIZE_BITOP];

public:
    redis_string(redis_client* client):redis_command(client){}
    ~redis_string(){}

public:
    /**
     * @description
     *   ����key���������ַ�����ֵ
     * @param [IN] key {const std::string&} �ַ��������key
     * @return {std::string} key���������ַ���ֵ�����key�����ڷ��ؿ��ַ���
     */
    std::string get(const std::string& key);

    /**
     * @description
     *   ��key�������ַ���ֵ����Ϊvalue��������key�������ľ�ֵ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ�����ֵ
     * @return {std::string} key���������ַ�����ֵ�����key�����ڷ��ؿ��ַ���
     */
    std::string getSet(const std::string& key, const std::string& value);

    /**
     * @description
     *   ��key�������ַ���value�����key���ڣ�������ԭ����ֵ, ���Һ���ԭʼ����
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�������false��ʾ����
     */
    bool set(const std::string& key, const std::string& value);

    /**
     * @description
     *   ���key�����ڣ���key�������ַ���value
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�������false��ʾ����
     */
    bool setnx(const std::string& key, const std::string& value);

    /**
     * @description
     *   ��key�������ַ���value, ��������key��ָ����ʱ��second֮��ʱ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] second {long long} ��ʱʱ��, ��λ:��
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�, ����false��ʾ����
     */
    bool setex(const std::string& key, long long second, const std::string& value);

    /**
     * @description
     *   ��key�������ַ���value, ��������key��ָ����ʱ��millisecond֮��ʱ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] second {long long} ��ʱʱ��, ��λ:����
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�, ����false��ʾ����
     */
    bool psetex(const std::string& key, long long millisecond, const std::string& value);

    /**
     * @description
     *   ����key��Ӧ���ַ������Ӵ�, �Ӵ���λ����start��end����(���߶����Ӵ���)
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] start {int} �Ӵ���ʼλ��
     * @param [IN] end {int} �Ӵ�ĩβλ��
     * @return {std::string} ����ָ�����Ӵ�
     */
    std::string getrange(const std::string& key, int start, int end);

    /**
     * @description
     *   ����key����Ӧ���ַ����Ĳ���ֵ, ��ָ����offset����ʼ, ����value�ĳ���.
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] offset {int} ָ��ƫ��������ʼλ��, ���ƫ���������ַ�������,
     *  ����������0���
     * @param [IN] value {const std::string&} ���ǵ�ֵ
     * @return {long long} ���ظ��Ǻ��ַ����ĳ���
     */
    long long setrange(const std::string& key, int offset, const std::string& value);

    /**
     * @description
     *   ����key����Ӧ���ַ�����offset����bitֵ. ���ƫ���������ַ�������, �򳬳�����
     *   ���ٶ�Ϊ��0bits���������ռ�. ��key�����ڵ�ʱ��, ������Ϊ��һ�����ַ���, 
     *   ����offset���ǳ�����Χ, ���ַ������ٶ�Ϊ��0bits���������ռ�.
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] offset {int} ָ��bitλ��ƫ����.
     *   ��һ���ֽڵ������ЧλΪ0, �ڶ��ֽ������ЧλΪ8, ��������.
     *   setbit��bitpos��bitλƫ�����������.
     * @return {int} ����ָ������λ��ֵ, ����ֵ����:
     *   0��1: ��������
     *   -1: ����
     */
    int getbit(const std::string& key, int offset);

    /**
     * @description
     *   ���û����key����Ӧ���ַ�����offset����bitλ. ���ƫ���������ַ�������,
     *   ���ַ����ͱ��ٶ�Ϊ��0bits���������ռ�. ��key�����ڵ�ʱ��, ������Ϊ��һ
     *   �����ַ���, ����offset���ǳ�����Χ, ���ַ������ٶ�Ϊ��0bits���������ռ�.
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] offset {int} ָ��bitλ��ƫ����. offset��Χ: [0, 2^32)
     * @param [IN] value {bool} ���û����offset����bitλ
     *   true: ����offset��bitλ
     *   false: ���offset��bitλ
     * @return {int} ����offset��ԭ����ֵ, ����ֵ����:
     *   0��1: ��������
     *   -1: ����
     */
    int setbit(const std::string& key, int offset, bool value);

    /**
     * @description
     *   ͳ��key����Ӧ���ַ�������Ϊ1��bitλ��, ��ָ��start��end, ��ֻͳ��
     *   [start, end]��������1��bitλ��
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] start {int} ��ʼ�ֽڵ�ƫ����
     * @param [IN] end {int} ĩβ�ֽڵ�ƫ����
     * @return {long long} ����Ϊ1��bitλ��
     *  >=0: ����Ϊ1��bitλ��
     *   -1: ����
     */
    long long bitconut(const std::string& key);
    long long bitconut(const std::string& key, int start, int end);

    /**
     * @description
     *   ��һ���������������λ���ַ���src_keys����λԪ����������������浽dest_key��
     * @param [IN] op {BITOP}
     *   λ������, ֻ��ʹ��ö��ֵ enum BITOP{ AND, OR, NOT, XOR}
     *   AND: ��һ������keyֵ���߼���, ����������浽dest_key��
     *    OR: ��һ������keyֵ���߼���, ����������浽dest_key��
     *   NOT: ��keyֵ�߼�ȡ��,  ����������浽dest_key��
     *   XOR: ��һ������keyֵ���߼����,  ����������浽dest_key��
     * @param [OUT] dest_key {const std::string&}
     *   ���������ַ�������
     * @param [IN] src_keys {const std::vector<std::string>&}
     *   ��Ҫ�����һ����һ���ַ�������
     * @return {long long} ���ر��浽dest_key���ֽڳ���
     *   >=0: ���浽dest_key���ֽڳ���
     *    -1: ����
     */
    long long bitop(BITOP op, const std::string& dest_key, const std::vector<std::string>& src_keys);

    /**
     * @description
     *   �����ַ��������һ��������Ϊvalue(0��1)��bitλ, ���ָ��start��end, ����[start, end]
     *   ���䷶Χ�ڼ���, start��end��ָ�ַ������ֽڵ�ƫ����, ����bitλ��ƫ����
     * @param [IN] key {const std::string&} ���������ַ��������key
     * @param [IN] value {bool} ��������bitֵ
     *   false: ����bitֵΪ0��λ
     *    true: ����bitֵΪ1��λ
     * @param [IN] start {int} ��������ʼ�ֽڵ�ƫ����, Ĭ��ֵΪ0, ��ʾ��ͷ��ʼ����
     * @param [IN] end {int} ������ĩβ�ֽڵ�ƫ����, Ĭ��ֵΪ-1, ��ʾһֱ�������ַ���ĩβ
     * @return {long long} �����ַ��������һ��������Ϊvalue(0��1)��bitλ
     *   >=0: ��һ��������Ϊvalue(0��1)��bitλ
     *    -1: ����
     */
    long long bitpos(const std::string& key, bool value, int start = 0, int end = -1);

    bool mget(std::vector<std::string>& keys, std::vector<std::string>& result);
    bool mget(std::vector<std::string>& keys, std::vector<std::string>* result);
    bool mset(std::map<std::string, std::string>& keyValues);
    bool msetnx(std::map<std::string, std::string>& keyValues);

    long long incr(std::string key);
    long long incrBy(std::string key, int increment);
    std::string incrByFloat(std::string key, double increment);
    long long decr(std::string key);
    long long decrBy(std::string key, int decrement);

    long long append(std::string key, std::string value);
    long long strlen(std::string key);
};

#endif /* __REDIS_STRING_H__ */
