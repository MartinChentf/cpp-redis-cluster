#ifndef __REDIS_KEY_H__
#define __REDIS_KEY_H__

#include <vector>
#include <string>

#include "redis_command.h"

class redis_key : public redis_command
{
public:
    redis_key(redis_client* client):redis_command(client){}
    ~redis_key(){}

public:
    /**
     * 删除指定的一个或一组keys, 忽略不存在的key
     * @param [IN] key {const std::vector<std::string>&} 指定的一组keys
     * @return {int} 返回实际删除的key的个数, 如下:
     *   0: 未删除任何key
     *  >0: 实际删除的key的个数, 该值可能小于输入key的个数
     *  -1: 出错
     */
    int del(const std::vector<std::string>& keys);
    int del(const std::string& key);

    /**
     * @description
     *   序列化给定key的值, 并返回序列化后的值. 使用RESTORE命令可将该值反序列化
     *   存入redis key中.
     * @param [IN] key {const std::string&} 给定的key
     * @param [OUT] result {std::string&} 存储序列化后的值
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 操作成功
     *    0: key不存在
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-04-22
     */
    int dump(const std::string& key, std::string& result);

    /**
     * @description
     *   判断key是否存在. 支持redis 3.0.3以上版本.
     * @param [IN] keys {const std::vector<std::string>&} 给定的key
     * @return {int} 返回存在的key的个数, 返回值如下:
     *   >0: 存在的key的个数, 如果参数中同一个key被重复多次, 则将会被多次计数
     *    0: key不存在
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int exists(const std::vector<std::string>& keys);

    /**
     * @description
     *   判断key是否存在.
     * @param [IN] key {const std::string&} 给定的key
     * @return {int} 返回key是否存在, 返回值如下:
     *    1: key存在
     *    0: key不存在
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int exists(const std::string& key);

    /**
     * @description
     *   设置key的生存时间. 生存时间超时, key将会被删除
     * @param [IN] key {const std::string&} 给定的key
     * @param [IN] second {int} 生存时间, 单位: 秒. 如果时间为非正数, key将被立
     *   即删除
     * @return {int} 返回值如下:
     *    1: 设置成功
     *    0: key不存在或生存时间无法被设置
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int expire(const std::string& key, int second);

    /**
     * @description
     *   用UNIX时间戳设置key的生存时间.
     * @param [IN] key {const std::string&} 给定的key
     * @param [IN] timestamp {time_t} UNIX时间戳. 如果时间戳表示过去的时刻, key
     *   将被立即删除
     * @return {int} 返回值如下:
     *    1: 设置成功
     *    0: key不存在或时间戳无法被设置
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int expireat(const std::string& key, time_t timestamp);

    /**
     * @description
     *   返回所有符合给定模式pattern的所有key.
     *   支持的正则表达模式:
     *    a) * 匹配所有key
     *    b) h?llo 匹配 hello, hallo 和 hxllo
     *    c) h*llo 匹配 hllo 和 heeeello
     *    d) h[ae]llo 匹配 hello 和 hallo, 但是不匹配 hillo
     *    e) h[^e]llo 匹配 hallo, hbllo, ... 但是不匹配 hello
     *    f) h[a-b]llo 匹配 hallo 和 hbllo
     * @param [IN] pattern {const std::string&} 匹配模式
     * @param [OUT] result {std::vector<std::string>&} 存储结果集
     * @return {int} 返回符合条件的key的个数, 返回值如下:
     *   >=0: 符合条件的key的个数
     *    -1: 出错
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int keys(const std::string& pattern, std::vector<std::string>& result);

    /**
     * @description
     *   将指定数据从当前的redis-server实例迁移到目标redis-server实例的指定数据
     *   库上. 该操作是一个原子操作, 当操作成功时, 可以确保数据从原实例删除并存
     *   在于目标实例中.
     *   选项:
     *      COPY - 数据不从原实例删除.
     *      REPLACE - 覆盖目标实例中已经存在的同名键值
     *      KEYS - 如果key参数为空字符串, 该命令一次可以迁移多个key, 迁移的键值
     *       由keys参数指定
     * @param [IN] host {const std::string&} 目标实例的服务器地址.
     * @param [IN] port {uint16_t} 目标实例的服务端口.
     * @param [IN] key {const std::string&} 需要迁动的数据的键值. 取值为 "" 时,
     *   表示使用 KEYS 选项.
     * @param [IN] dest_db {int} 目标实例的数据库ID号.
     * @param [IN] timeout {long long} 迁移过程的超时时间, 单位: 毫秒.
     * @param [IN] is_copy {bool} 是否使用 COPY 选项, 默认值为false.
     * @param [IN] is_replace {bool} 是否使用 REPLACE 选项, 默认值为false.
     * @param [IN] keys {const std::vector<std::string>*} 是否使用 KEYS 选项, 默
     *   认值为NULL. 当key = "" , 并且keys不为空时, 启用该选项. 
     * @return {bool} 迁移是否成功.
     * @author chen.tengfei
     * @date 2017-04-26
     */
    bool migrate(const std::string& host, uint16_t port, const std::string& key,
        int dest_db, long long timeout, bool is_copy = false,
        bool is_replace = false, const std::vector<std::string>* keys = NULL);

    /**
     * @description
     *   将指定key从当前数据库移动到指定数据库. 该命令禁止在集群模式下使用.
     * @param [IN] key {const std::string&} 指定移动的键值
     * @param [IN] db {int} 目标数据库
     * @return {int} 是否移动成功, 返回值如下:
     *    1: 迁移成功
     *    0: 未迁移成功, 目标数据库有相同名字的给定key,或key不存在于当前数据库中
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-04-26
     */
    bool move(const std::string& key, int db);
    /**
     * @description
     *   用于迭代当前选择的redis数据库中key的集合
     * @param [IN] cursor {int} 游标值, 第一次迭代使用0作为游标.
     * @param [OUT] result {std::vector<std::string>&} 存储结果集, 内部以追加方
     *   式将本次遍历结果添加进该对象中, 为防止因总结果集过大导致该数组溢出, 用
     *   户可在调用本函数前后清理该对象.
     * @param [IN] pattern {const char*} glob风格的模式参数, 非空时有效
     * @param [IN] count {int} 限定结果集元素的数量, 默认值:10
     * @return {int} 下一个游标位置, 返回值如下:
     *   >0: 下一个游标位置
     *    0: 遍历结束
     *   -1: key的value类型错误(non-hash)
     */
    int scan(int cursor, std::vector<std::string>& result,
             const char* pattern = NULL, int count = 10);
};

#endif /* __REDIS_KEY_H__ */
