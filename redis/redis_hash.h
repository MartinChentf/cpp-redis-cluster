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
    bool hgetall(const std::string& key,
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
     * @return {bool} 表示自增操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: 1) key的value类型错误(non-hash)
     *          2) field关联的值不能解析成双精度浮点值的字符串
     */
    bool hincrbyfloat(const std::string& key, const std::string& field,
                      double increment, std::string* result = NULL);

    /**
     * @description
     *   返回key关联的哈希集中所有field域的名字, 如果key不存在返回空集
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [OUT] result {std::vector<std::string>&} 哈希集中所有field域的名字
     * @return {bool} 表示操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: key的value类型错误(non-hash)
     */
    bool hkeys(const std::string& key, std::vector<std::string>& result);

    /**
     * @description
     *   返回key关联的哈希集中域的数量, 如果key不存在返回0
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @return {long long} 哈希集中域的数量, 返回值如下:
     *   >0: 哈希集中域的数量
     *    0: key不存在
     *   -1: key的value类型错误(non-hash)
     */
    long long hlen(const std::string& key);

    /**
     * @description
     *   返回key关联的哈希集中指定的field域的值集合. 
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] fields {const std::vector<std::string>&} 指定的域集合
     * @param [OUT] result {std::vector<std::string*>&} 指定域关联的值. 如果key
     *   不存在或指定的域不存在, 返回集合的对应位置为空指针.
     * @return {bool} 表示操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: key的value类型错误(non-hash)
     */
    bool hmget(const std::string& key, const std::vector<std::string>& fields,
               std::vector<std::string*>& result);

    /**
     * @description
     *   设置key关联的哈希集中指定的field域的值. 如果指定的field域已存在, 则覆盖
     *   原来的旧值. 如果key不存在, 则先创建一个空的哈希集.
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] field_values {const std::map<std::string, std::string>&}
     *   给定的域值对的集合
     * @return {bool} 表示操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: key的value类型错误(non-hash)
     */
    bool hmset(const std::string& key,
               const std::map<std::string, std::string>& field_values);

    /**
     * @description
     *   用于迭代key关联的哈希集中的键值对
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] cursor {int} 游标值, 第一次迭代使用0作为游标.
     * @param [OUT] result {std::map<std::string, std::string>&} 存储结果集, 内
     *   部以追加方式将本次遍历结果添加进该对象中, 为防止因总结果集过大导致该数
     *   组溢出, 用户可在调用本函数前后清理该对象.
     * @param [IN] pattern {const char*} glob风格的模式参数, 非空时有效
     * @param [IN] count {int} 限定结果集元素的数量, 默认值:10
     * @return {int} 下一个游标位置, 返回值如下:
     *   >0: 下一个游标位置
     *    0: 遍历结束
     *   -1: key的value类型错误(non-hash)
     */
    int hscan(const std::string& key, int cursor,
              std::map<std::string, std::string>& result,
              const char* pattern = NULL, int count = 10);

    /**
     * @description
     *   将key关联的哈希集中指定的field域的值设置成value. 如果指定的field域已存
     *   在, 则覆盖原来的旧值. 如果key不存在, 则先创建一个空的哈希集.
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] field {const std::string&} 指定要设置的域
     * @param [IN] value {const std::string&} 指定的域对应的值
     * @return {int} 返回操作结果, 返回值如下:
     *    1: field域是新的域或者key不存在, 则设置其值
     *    0: field域存在并更新其值
     *   -1: key的value类型错误(non-hash)
     */
    int hset(const std::string& key, const std::string& field,
             const std::string& value);

    /**
     * @description
     *   将key关联的哈希集中指定的field域的值设置成value. 如果指定的field域不存
     *   在, 则设置field域的值为value. 如果key不存在, 则先创建一个空的哈希集. 如
     *   果field已存在, 则该操作无任何影响.
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] field {const std::string&} 指定要设置的域
     * @param [IN] value {const std::string&} 指定的域对应的值
     * @return {int} 返回操作结果, 返回值如下:
     *    1: field域是新的域或者key不存在, 则设置其值
     *    0: field域存在则不执行操作
     *   -1: key的value类型错误(non-hash)
     */
    int hsetnx(const std::string& key, const std::string& field,
               const std::string& value);

    /**
     * @description
     *   返回key关联的哈希集中指定的field域的值的字符串长度. 如果指定的field域不
     *   存在或者key不存在, 则返回0.
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [IN] field {const std::string&} 指定要设置的域
     * @return {int} 返回操作结果, 返回值如下:
     *   >0: field域的值的字符串长度
     *    0: field域不存在或者key不存在
     *   -1: key的value类型错误(non-hash)
     */
    long long hstrlen(const std::string& key, const std::string& field);

    /**
     * @description
     *   返回key关联的哈希集中所有值的集合, 如果key不存在返回空列表
     * @param [IN] key {const std::string&} 哈希集对象的key
     * @param [OUT] result {std::vector<std::string>&} 哈希集中所有的值
     * @return {bool} 表示操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: key的value类型错误(non-hash)
     */
    bool hvals(const std::string& key, std::vector<std::string>& result);
};

#endif /* __REDIS_HASH_H__ */
