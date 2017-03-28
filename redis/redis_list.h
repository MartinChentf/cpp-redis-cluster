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
    //blpop
    //brpop
    std::string brpoplpush(std::string src, std::string dest, long long timeout);
    /**
     * @description
     *   返回key关联的列表中下标为index的元素
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] index {int} 下标值
     * @param [OUT] result {std::string&} 存储结果
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 返回下标为index的元素
     *    0: 下标超出范围
     *   -1: 出错或key的value类型错误(non-list)
     */
    int lindex(const std::string& key, int index, std::string& result);

    /**
     * @description
     *   在key关联的列表中将一个新元素value添加到元素pivot之前/后
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] is_before {bool} 指定插入到元素pivot之前/后
     * @param [IN] pivot {const std::string&} 指定的元素
     * @param [IN] value {const std::string&} 新元素
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 插入新元素后, 当前列表元素个数
     *    0: key不存在, 无操作
     *   -1: 1) key的value类型错误(non-list)
     *       2) 元素pivot不在列表中
     */
    long long linsert(const std::string& key, bool is_before,
                      const std::string& pivot, const std::string& value);

    /**
     * @description
     *   返回key关联的列表的长度
     * @param [IN] key {const std::string&} 列表对象的key
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 列表长度
     *    0: key不存在
     *   -1: key的value类型错误(non-list)
     */
    long long llen(const std::string& key);

    /**
     * @description
     *   弹出并返回key关联的列表中的第一个元素
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [OUT] result {std::string&} 存储弹出的元素
     * @return {long long} 返回操作结果, 返回值如下:
     *    1: 成功弹出列表中元素
     *    0: key不存在
     *   -1: key的value类型错误(non-list)
     */
    int lpop(const std::string& key, std::string& result);

    /**
     * @description
     *   向key关联的列表头部依次插入values中所有的元素, 插入后的元素顺序和values
     *   中的相反. 如果key不存在, 插入元素前先创建一个新列表
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] values {const std::vector<std::string>&} 新元素集合
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 插入新元素后, 当前列表元素个数
     *   -1: 出错或key的value类型错误(non-list)
     */
    long long lpush(const std::string& key,
                    const std::vector<std::string>& values);

    /**
     * @description
     *   如果key存在并且是列表对象, 则向key关联的列表头部插入元素value. 如果key
     *   不存在, 则不插入元素
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] value {const std::string&} 新元素
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 插入新元素后, 当前列表元素个数
     *   -1: 出错或key的value类型错误(non-list)
     */
    long long lpushx(const std::string& key, const std::string& value);

    /**
     * @description
     *   返回key关联的列表中[start, end](闭区间)范围内的元素. start和end可为负数,
     *   -1表示最后一个元素, -2表示倒数第二个元素
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] start {int} 起始下标
     * @param [IN] end {int} 结束下标
     * @param [OUT] result {std::vector<std::string>&} 存储指定区间的元素集合
     * @return {bool} 操作是否成功, false表示出错或key的value类型错误(non-list)
     */
    bool lrange(const std::string& key, int start, int stop,
                std::vector<std::string>& result);

    long long lrem(std::string key, int count, std::string value);
    bool lset(std::string key, int index, std::string value);
    bool ltrim(std::string key, int start, int stop);

    std::string rpop(std::string key);
    std::string rpoplpush(std::string src, std::string dest);
    long long rpush(std::string key, std::vector<std::string>& values);
    long long rpushx(std::string key, std::string value);
    
};

#endif /* __REIDS_LIST_H__ */
