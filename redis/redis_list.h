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
     *   blpop函数是阻塞式列表的弹出原语. 它是lpop函数的阻塞版本.
     *   1) 当给定的所有列表没有任何元素的时候, 连接将被blpop函数阻塞, 直到别的
     *      客户端向这些列表中的任意一个列表push元素或者超时. 当timeout为0时, 表
     *      示无限期阻塞
     *   2) 如果给定的列表中至少有一个非空列表, 则弹出第一个非空列表的头部元素.
     * @param [IN] keys {const std::vector<std::string>&} 给定弹出元素的列表组
     * @param [IN] timeout {long long} 超时时间
     * @param [OUT] result {std::map<std::string, std::string>&}
     *   存储弹出元素的key以及对应的元素
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 操作成功
     *    0: 超时
     *   -1: 1) src或dest的value类型错误(non-list)
     *       2) 其他错误
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int blpop(const std::vector<std::string>& keys, long long timeout,
              std::map<std::string, std::string>& result);

    /**
     * @description
     *   brpop函数是阻塞式列表的弹出原语. 它是rpop函数的阻塞版本. 具体行为与
     *   blpop函数类似, 除了brpop函数是从列表尾部弹出元素.
     * @param [IN] keys {const std::vector<std::string>&} 给定弹出元素的列表组
     * @param [IN] timeout {long long} 超时时间
     * @param [OUT] result {std::map<std::string, std::string>&}
     *   存储弹出元素的key以及对应的元素
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 操作成功
     *    0: 超时
     *   -1: 1) src或dest的value类型错误(non-list)
     *       2) 其他错误
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int brpop(const std::vector<std::string>& keys, long long timeout,
              std::map<std::string, std::string>& result);
    /**
     * @description
     *   brpoplpush函数是rpoplpush函数的阻塞版本. 当src包含元素时, 行为和
     *   rpoplpush函数一样. 当src为空时, redis将会阻塞该连接, 直到别的客户端向
     *   src中push元素或者超时. 
     * @param [IN] src {const std::string&} 源列表对象的键值
     * @param [IN] dest {const std::string&} 目的列表对象的键值
     * @param [IN] timeout {long long} 超时时间
     * @param [OUT] result {std::string&} 存储弹出的元素
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 操作成功
     *    0: 超时
     *   -1: 1) src或dest的value类型错误(non-list)
     *       2) 其他错误
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int brpoplpush(const std::string& src, const std::string& dest,
                   long long timeout, std::string& result);
    /**
     * @description
     *   返回key关联的列表中下标为index的元素. index为基于0的下标, 可为负数, 
     *   -1表示最后一个元素, -2表示倒数第二个元素.
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
     * @return {int} 返回操作结果, 返回值如下:
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
     *   返回key关联的列表中[start, end](闭区间)范围内的元素. start和end为基于0
     *   的下标, 可为负数, -1表示最后一个元素, -2表示倒数第二个元素
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] start {int} 起始下标
     * @param [IN] end {int} 结束下标
     * @param [OUT] result {std::vector<std::string>&} 存储指定区间的元素集合
     * @return {bool} 操作是否成功, false表示出错或key的value类型错误(non-list)
     */
    bool lrange(const std::string& key, int start, int stop,
                std::vector<std::string>& result);

    /**
     * @description
     *   移除key关联的列表中前count个值等于value的元素.
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] count {int} 移除元素的个数. count的取值如下:
     *   count > 0: 从列表头部开始移除值等于value元素
     *   count < 0: 从列表尾部部开始移除值等于value元素
     *   count = 0: 移除列表中所有等于value元素
     * @param [IN] value {const std::string&} 需要移除的元素
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 移除元素的个数
     *    0: key不存在或value不存在
     *   -1: 出错或key的value类型错误(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    long long lrem(const std::string& key, int count, const std::string& value);

    /**
     * @description
     *   设置key关联的列表中下标index处的值为value. index取值请参考lindex函数.
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] index {int} 元素下标
     * @param [IN] value {const std::string&} 新的元素
     * @return {bool} 返回操作结果, 返回值如下:
     *    true: 设置新值成功
     *   false: 1) key的value类型错误(non-list)
     *          2) index越界
     * @author chen.tengfei
     * @date 2017-03-29
     */
    bool lset(const std::string& key, int index, const std::string& value);

    /**
     * @description
     *   修剪key关联的列表使其只包含[start, stop]指定范围内的元素. 如果start大于
     *   列表末尾下标或者start > stop, 结果将为空列表. 这将造成key被删除. 如果
     *   stop大于列表下标, 则会被当做列表末尾下标
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] start {int} 起始下标
     * @param [IN] stop {int} 结束下标
     * @return {bool} 返回操作结果, 返回值如下:
     *    true: 操作成功
     *   false: key的value类型错误(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    bool ltrim(const std::string& key, int start, int stop);

    /**
     * @description
     *   弹出并返回key关联的列表中的最后一个元素
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [OUT] result {std::string&} 存储弹出的元素
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 成功弹出列表中元素
     *    0: key不存在
     *   -1: key的value类型错误(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int rpop(const std::string& key, std::string& result);

    /**
     * @description
     *   弹出并返回src列表中的最后一个元素, 并将其放入dest列表的头部. 该操作是原
     *   子操作. 如果src不存在, 则返回空且不做任何操作. 如果src和dest相同, 则返
     *   回列表末尾元素并将该元素移到列表头部.
     * @param [IN] src {const std::string&} 列表对象的src
     * @param [IN] dest {const std::string&} 列表对象的dest
     * @param [OUT] result {std::string&} 存储弹出的元素
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 操作成功
     *    0: src不存在
     *   -1: 1) src或dest的value类型错误(non-list)
     *       2) 其他错误
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int rpoplpush(const std::string& src, const std::string& dest,
                  std::string& result);

    /**
     * @description
     *   向key关联的列表尾部依次插入values中所有的元素. 如果key不存在, 插入元素
     *   前先创建一个新列表
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] values {const std::vector<std::string>&} 新元素集合
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 插入新元素后, 当前列表元素个数
     *   -1: 出错或key的value类型错误(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    long long rpush(const std::string& key,
                    const std::vector<std::string>& values);

    /**
     * @description
     *   如果key存在并且是列表对象, 则向key关联的列表尾部插入元素value. 如果key
     *   不存在, 则不插入元素
     * @param [IN] key {const std::string&} 列表对象的key
     * @param [IN] value {const std::string&} 新元素
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 插入新元素后, 当前列表元素个数
     *   -1: 出错或key的value类型错误(non-list)
     * @author chen.tengfei
     * @date 2017-03-29
     */
    long long rpushx(const std::string& key, const std::string& value);
    
};

#endif /* __REIDS_LIST_H__ */
