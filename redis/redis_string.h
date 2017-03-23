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
     *   返回key所关联的字符串的值
     * @param [IN] key {const std::string&} 字符串对象的key
     * @return {std::string} key所关联的字符串值，如果key不存在返回空字符串
     */
    std::string get(const std::string& key);

    /**
     * @description
     *   将key关联的字符串值设置为value，并返回key所关联的旧值
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串新值
     * @return {std::string} key所关联的字符串旧值，如果key不存在返回空字符串
     */
    std::string getSet(const std::string& key, const std::string& value);

    /**
     * @description
     *   将key关联到字符串value，如果key存在，将覆盖原来的值, 并且忽略原始类型
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功，返回false表示出错
     */
    bool set(const std::string& key, const std::string& value);

    /**
     * @description
     *   如果key不存在，将key关联到字符串value
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功，返回false表示出错
     */
    bool setnx(const std::string& key, const std::string& value);

    /**
     * @description
     *   将key关联到字符串value, 并且设置key在指定的时间second之后超时
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] second {long long} 超时时间, 单位:秒
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功, 返回false表示出错
     */
    bool setex(const std::string& key, long long second, const std::string& value);

    /**
     * @description
     *   将key关联到字符串value, 并且设置key在指定的时间millisecond之后超时
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] second {long long} 超时时间, 单位:毫秒
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功, 返回false表示出错
     */
    bool psetex(const std::string& key, long long millisecond, const std::string& value);

    /**
     * @description
     *   返回key对应的字符串的子串, 子串由位移量start和end决定(两者都在子串内)
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] start {int} 子串起始位移
     * @param [IN] end {int} 子串末尾位移
     * @return {std::string} 返回指定的子串
     */
    std::string getrange(const std::string& key, int start, int end);

    /**
     * @description
     *   覆盖key所对应的字符串的部分值, 从指定的offset处开始, 覆盖value的长度.
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] offset {int} 指定偏移量的起始位置, 如果偏移量大于字符串长度,
     *  超出部分用0填充
     * @param [IN] value {const std::string&} 覆盖的值
     * @return {long long} 返回覆盖后字符串的长度
     */
    long long setrange(const std::string& key, int offset, const std::string& value);

    /**
     * @description
     *   返回key所对应的字符串在offset处的bit值. 如果偏移量超出字符串长度, 则超出部分
     *   被假定为由0bits填充的连续空间. 当key不存在的时候, 它就认为是一个空字符串, 
     *   所以offset总是超出范围, 则字符串被假定为由0bits填充的连续空间.
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] offset {int} 指定bit位的偏移量.
     *   第一个字节的最高有效位为0, 第二字节最高有效位为8, 依此类推.
     *   setbit和bitpos的bit位偏移量与此相似.
     * @return {int} 返回指定比特位的值, 返回值如下:
     *   0或1: 正常返回
     *   -1: 出错
     */
    int getbit(const std::string& key, int offset);

    /**
     * @description
     *   设置或清除key所对应的字符串在offset处的bit位. 如果偏移量超出字符串长度,
     *   该字符串就被假定为由0bits填充的连续空间. 当key不存在的时候, 它就认为是一
     *   个空字符串, 所以offset总是超出范围, 则字符串被假定为由0bits填充的连续空间.
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] offset {int} 指定bit位的偏移量. offset范围: [0, 2^32)
     * @param [IN] value {bool} 设置或清除offset处的bit位
     *   true: 设置offset处bit位
     *   false: 清除offset处bit位
     * @return {int} 返回offset处原比特值, 返回值如下:
     *   0或1: 正常返回
     *   -1: 出错
     */
    int setbit(const std::string& key, int offset, bool value);

    /**
     * @description
     *   统计key所对应的字符串被置为1的bit位数, 若指定start和end, 则只统计
     *   [start, end]区间内置1的bit位数
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] start {int} 起始字节的偏移量
     * @param [IN] end {int} 末尾字节的偏移量
     * @return {long long} 被置为1的bit位数
     *  >=0: 被置为1的bit位数
     *   -1: 出错
     */
    long long bitconut(const std::string& key);
    long long bitconut(const std::string& key, int start, int end);

    /**
     * @description
     *   对一个或多个保存二进制位的字符串src_keys进行位元操作，并将结果保存到dest_key中
     * @param [IN] op {BITOP}
     *   位操作符, 只能使用枚举值 enum BITOP{ AND, OR, NOT, XOR}
     *   AND: 对一个或多个key值求逻辑与, 并将结果保存到dest_key中
     *    OR: 对一个或多个key值求逻辑或, 并将结果保存到dest_key中
     *   NOT: 对key值逻辑取反,  并将结果保存到dest_key中
     *   XOR: 对一个或多个key值求逻辑异或,  并将结果保存到dest_key中
     * @param [OUT] dest_key {const std::string&}
     *   保存结果的字符串对象
     * @param [IN] src_keys {const std::vector<std::string>&}
     *   需要计算的一个或一组字符串对象
     * @return {long long} 返回保存到dest_key的字节长度
     *   >=0: 保存到dest_key的字节长度
     *    -1: 出错
     */
    long long bitop(BITOP op, const std::string& dest_key, const std::vector<std::string>& src_keys);

    /**
     * @description
     *   返回字符串里面第一个被设置为value(0或1)的bit位, 如果指定start或end, 则在[start, end]
     *   区间范围内检索, start和end是指字符串中字节的偏移量, 不是bit位的偏移量
     * @param [IN] key {const std::string&} 被检索的字符串对象的key
     * @param [IN] value {bool} 被检索的bit值
     *   false: 检索bit值为0的位
     *    true: 检索bit值为1的位
     * @param [IN] start {int} 检索的起始字节的偏移量, 默认值为0, 表示从头开始检索
     * @param [IN] end {int} 检索的末尾字节的偏移量, 默认值为-1, 表示一直检索到字符串末尾
     * @return {long long} 返回字符串里面第一个被设置为value(0或1)的bit位
     *   >=0: 第一个被设置为value(0或1)的bit位
     *    -1: 出错
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
