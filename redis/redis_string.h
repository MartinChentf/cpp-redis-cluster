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
     * 返回key所关联的字符串的值
     * @param [IN] key {const std::string&} 字符串对象的key
     * @return {std::string} key所关联的字符串值，如果key不存在返回空字符串
     */
    std::string get(const std::string& key);

    /**
     * 将key关联的字符串值设置为value，并返回key所关联的旧值
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串新值
     * @return {std::string} key所关联的字符串旧值，如果key不存在返回空字符串
     */
    std::string getSet(const std::string& key, const std::string& value);

    /**
     * 将key关联到字符串value，如果key存在，将覆盖原来的值, 并且忽略原始类型
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功，返回false表示出错
     */
    bool set(const std::string& key, const std::string& value);

    /**
     * 如果key不存在，将key关联到字符串value
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功，返回false表示出错
     */
    bool setnx(const std::string& key, const std::string& value);

    /**
     * 将key关联到字符串value, 并且设置key在指定的时间second之后超时
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] second {long long} 超时时间, 单位:秒
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功, 返回false表示出错
     */
    bool setex(const std::string& key, long long second, const std::string& value);

    /**
     * 将key关联到字符串value, 并且设置key在指定的时间millisecond之后超时
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] second {long long} 超时时间, 单位:毫秒
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功, 返回false表示出错
     */
    bool psetex(const std::string& key, long long millisecond, const std::string& value);

    /**
     * 返回key对应的字符串的子串, 子串由位移量start和end决定(两者都在子串内)
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] start {int} 子串起始位移
     * @param [IN] end {int} 子串末尾位移
     * @return {std::string} 返回指定的子串
     */
    std::string getrange(const std::string& key, int start, int end);

    /**
     * 覆盖key所对应的字符串的部分值, 从指定的offset处开始, 覆盖value的长度.
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] offset {int} 指定偏移量的起始位置, 如果偏移量大于字符串长度,
     *  超出部分用0填充
     * @param [IN] value {const std::string&} 覆盖的值
     * @return {long long} 返回覆盖后字符串的长度
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
