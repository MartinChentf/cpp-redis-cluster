#ifndef __REDIS_SET_H__
#define __REDIS_SET_H__

#include <string>
#include <vector>

#include "redis_command.h"

class redis_client;

class redis_set : public redis_command
{
public:
    redis_set(redis_client* client):redis_command(client){}
    ~redis_set(){}

public:
    /**
     * @description
     *   向key关联的集合中添加指定的成员. 如果指定的成员已经存在, 则忽略. 如果
     *   key不存在, 先创建一个新的集合.
     * @param [IN] key {const std::string&} 集合对象的key
     * @param [IN] member {const std::vector<std::string>&} 指定添加的成员列表
     * @return {long long} 返回操作结果, 返回值如下:
     *   >=0: 新增加的元素个数, 不包括已经存在的集合元素
     *    -1: 出错或key的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sadd(const std::string& key,
                   const std::vector<std::string>& member);

    /**
     * @description
     *   返回key关联的集合的基数(集合的元素个数)
     * @param [IN] key {const std::string&} 集合对象的key
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 集合的基数
     *    0: key不存在
     *   -1: 出错或key的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long scard(const std::string& key);

    /**
     * @description
     *   返回指定的一组集合中第一个集合与其余集合的差集. 不存在的key被当做空集.
     * @param [IN] keys {const std::vector<std::string>&} 指定的一组集合
     * @param [OUT] result {std::vector<std::string>&} 存储返回的差集
     * @return {bool} 返回操作结果, false表示出错或keys的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sdiff(const std::vector<std::string>& keys,
               std::vector<std::string>& result);

    /**
     * @description
     *   返回所有给定集合的交集. 不存在的key被当做空集.
     * @param [IN] keys {const std::vector<std::string>&} 指定的一组集合
     * @param [OUT] result {std::vector<std::string>&} 存储返回的交集
     * @return {bool} 返回操作结果, false表示出错或keys的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sinter(const std::vector<std::string>& keys,
                std::vector<std::string>& result);

    /**
     * @description
     *   返回所有给定集合的并集. 不存在的key被当做空集.
     * @param [IN] keys {const std::vector<std::string>&} 指定的一组集合
     * @param [OUT] result {std::vector<std::string>&} 存储返回的并集
     * @return {bool} 返回操作结果, false表示出错或keys的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sunion(const std::vector<std::string>& keys,
                std::vector<std::string>& result);

    /**
     * @description
     *   返回指定的一组集合中第一个集合与其余集合的差集, 并把结果存储在集合dest
     *   中. 如果dest集合已存在, 则覆盖原有元素. 不存在的key被当做空集.
     * @param [IN] dest {const std::string&} 存放计算结果的集合对象的key
     * @param [IN] keys {const std::vector<std::string>&} 指定的一组集合
     * @return {long long} 返回结果集的元素个数, 返回值如下:
     *   >=0: 结果集的元素个数
     *    -1: 表示出错或集合对象的类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sdiffstore(const std::string& dest,
                         const std::vector<std::string>& keys);

    /**
     * @description
     *   返回所有给定集合的交集, 并把结果存储在集合dest中. 如果dest集合已存在,
     *   则覆盖原有元素. 不存在的key被当做空集.
     * @param [IN] dest {const std::string&} 存放计算结果的集合对象的key
     * @param [IN] keys {const std::vector<std::string>&} 指定的一组集合
     * @return {long long} 返回结果集的元素个数, 返回值如下:
     *   >=0: 结果集的元素个数
     *    -1: 表示出错或集合对象的类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sinterstore(const std::string& dest,
                          const std::vector<std::string>& keys);

    /**
     * @description
     *   返回所有给定集合的并集, 并把结果存储在集合dest中. 如果dest集合已存在,
     *   则覆盖原有元素. 不存在的key被当做空集.
     * @param [IN] dest {const std::string&} 存放计算结果的集合对象的key
     * @param [IN] keys {const std::vector<std::string>&} 指定的一组集合
     * @return {long long} 返回结果集的元素个数, 返回值如下:
     *   >=0: 结果集的元素个数
     *    -1: 表示出错或集合对象的类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sunionstore(const std::string& dest,
                          const std::vector<std::string>& keys);

    bool sismember(const std::string key, const std::string member);
    bool smember(const std::string key, std::vector<std::string>& result);
    bool smove(const std::string src, const std::string dest, const std::string member);
    bool spop(const std::string key, std::vector<std::string>& result, int count = -1);
    bool srandmember(const std::string key, std::vector<std::string>& result, int count = -1);
    long long srem(const std::string key, const std::vector<std::string>& member);
    bool sscan(std::string key, int& cursor, std::vector<std::string>& result);

private:
    bool set_operation(const char* op, const std::vector<std::string>& keys,
                       std::vector<std::string>& result);
    long long set_operation_with_store(const char * op, const std::string& dest,
                                       const std::vector<std::string>& keys);
};

#endif /* __REDIS_SET_H__ */
