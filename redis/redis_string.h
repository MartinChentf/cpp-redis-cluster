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
     * ����key���������ַ�����ֵ
     * @param [IN] key {const std::string&} �ַ��������key
     * @return {std::string} key���������ַ���ֵ�����key�����ڷ��ؿ��ַ���
     */
    std::string get(const std::string& key);

    /**
     * ��key�������ַ���ֵ����Ϊvalue��������key�������ľ�ֵ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ�����ֵ
     * @return {std::string} key���������ַ�����ֵ�����key�����ڷ��ؿ��ַ���
     */
    std::string getSet(const std::string& key, const std::string& value);

    /**
     * ��key�������ַ���value�����key���ڣ�������ԭ����ֵ, ���Һ���ԭʼ����
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�������false��ʾ����
     */
    bool set(const std::string& key, const std::string& value);

    /**
     * ���key�����ڣ���key�������ַ���value
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�������false��ʾ����
     */
    bool setnx(const std::string& key, const std::string& value);

    /**
     * ��key�������ַ���value, ��������key��ָ����ʱ��second֮��ʱ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] second {long long} ��ʱʱ��, ��λ:��
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�, ����false��ʾ����
     */
    bool setex(const std::string& key, long long second, const std::string& value);

    /**
     * ��key�������ַ���value, ��������key��ָ����ʱ��millisecond֮��ʱ
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] second {long long} ��ʱʱ��, ��λ:����
     * @param [IN] value {const std::string&} key���������ַ���ֵ
     * @return {bool} �����Ƿ�ɹ�, ����false��ʾ����
     */
    bool psetex(const std::string& key, long long millisecond, const std::string& value);

    /**
     * ����key��Ӧ���ַ������Ӵ�, �Ӵ���λ����start��end����(���߶����Ӵ���)
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] start {int} �Ӵ���ʼλ��
     * @param [IN] end {int} �Ӵ�ĩβλ��
     * @return {std::string} ����ָ�����Ӵ�
     */
    std::string getrange(const std::string& key, int start, int end);

    /**
     * ����key����Ӧ���ַ����Ĳ���ֵ, ��ָ����offset����ʼ, ����value�ĳ���.
     * @param [IN] key {const std::string&} �ַ��������key
     * @param [IN] offset {int} ָ��ƫ��������ʼλ��, ���ƫ���������ַ�������,
     *  ����������0���
     * @param [IN] value {const std::string&} ���ǵ�ֵ
     * @return {long long} ���ظ��Ǻ��ַ����ĳ���
     */
    long long setrange(const std::string& key, int offset, const std::string& value);

    long long getbit(std::string key, int offset);
    long long setbit(std::string key, int offset, bool value);
    long long bitconut(std::string key);
    long long bitconut(std::string key, int start, int end);
    long long bitop(BITOP op, std::string dest_key, std::vector<std::string>& src_keys);
    long long bitpos(std::string key, bool value, int start = 0, int end = -1);

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
