#ifndef __REDIS_HASH_H__
#define __REDIS_HASH_H__

#include <map>
#include <string>
#include <vector>

#include "redis_command.h"

class redis_client;

class redis_hash : public redis_command
{
public:
    typedef std::map<std::string, std::string> t_field_value_map;
    typedef t_field_value_map::iterator t_field_value_map_iter;

public:
    redis_hash(redis_client* client):redis_command(client){}
    ~redis_hash(){}

public:
    /**
     * @description
     *   从key关联的哈希集中删除fields列表指定的域
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] fields {const std::vector<std::string>&} 指定要删除的域
     * @return {long long} 实际删除的域的数量, 不包括指定删除但实际不存在的域
     *   >0: 删除的域的数量
     *   =0: key不存在或指定的域不存在
     *   -1: key的value类型错误(non-hash)
     */
    long long hdel(const std::string& key,
                   const std::vector<std::string>& fields);

    /**
     * @description
     *   判断key关联的哈希集中field域是否存在
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] field {const std::string&} 指定的域
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 给定的域存在
     *    0: key不存在或指定的域不存在
     *   -1: 出错或key的value类型错误(non-hash)
     */
    int hexists(const std::string& key, const std::string& field);

    /**
     * @description
     *   返回key关联的哈希集中field域关联的值
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] field {const std::string&} 指定的域
     * @param [OUT] result {const std::string&} 指定域关联的值. 如果key不存在或
     *   指定的与不存在, 返回空串. 此时需要根据返回值判断指定域是否存在, 因为有
     *   可能指定域关联的字符串是空串
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 给定的域存在
     *    0: key不存在或指定的域不存在
     *   -1: 出错或key的value类型错误(non-hash)
     */
    int hget(const std::string& key, const std::string& field,
             std::string& result);

    /**
     * @description
     *   返回key关联的哈希集中所有域及其关联的值
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [OUT] result {std::map<std::string, std::string>&}
     *   所有域以及关联的值的映射表. 如果key不存在, 则映射表为空
     * @return {bool} 操作是否成功, false表示出错或key的value类型错误(non-hash)
     */
    bool hgetall(const std::string key,
                 std::map<std::string, std::string>& result);

    /**
     * @description
     *   使key关联的哈希集中field域增加increment. 如果key不存在, 则创建一个关联
     *   于key的空哈希集. 如果field不存在, 则自增操作前先将value设置成0
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] field {const std::string&} 指定自增的域
     * @param [IN] increment {long long} 自增量, <0表示自减
     * @param [OUT] result {long long*} 非空时存储自增结果
     * @return {bool} 表示自增操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: 1) key的value类型错误(non-hash)
     *          2) field关联的值是不能表示成数字的字符串
     */
    bool hincrby(const std::string& key, const std::string& field,
                 long long increment, long long* result = NULL);

    /**
     * @description
     *   使key关联的哈希集中field域增加increment. 如果key不存在, 则创建一个关联
     *   于key的空哈希集. 如果field不存在, 则自增操作前先将value设置成0
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] field {const std::string&} 指定自增的域
     * @param [IN] increment {double} 增量值, <0表示自减
     * @param [OUT] result {std::string*} 非空时存储自增结果
     * @return {bool} 表示自增操作是否成功, 如下:
     *    true: 操作成功
     *   false: 1) key的value类型错误(non-hash)
     *          2) field关联的值不能解析成双精度浮点值的字符串
     */
    bool hincrbyfloat(std::string key, std::string field,
                      double increment, std::string* result = NULL);

    bool hkeys(std::string key, std::vector<std::string>& result);
    long long hlen(std::string key);
    bool hmget(std::string key, std::vector<std::string>& fields, std::vector<std::string>& result);
    bool hmset(std::string key, std::map<std::string, std::string>& field_values);
    bool hscan(std::string key, int& cursor, std::map<std::string, std::string>& result);
    bool hset(std::string key, std::string field, std::string value);
    bool hsetnx(std::string key, std::string field, std::string value);
    long long hstrlen(std::string key, std::string field);
    bool hvals(std::string key, std::vector<std::string>& result);
};

#endif /* __REDIS_HASH_H__ */
