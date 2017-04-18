#ifndef __REDIS_SET_H__
#define __REDIS_SET_H__

#include <string>
#include <vector>

#include "redis_command.h"

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
     *    -1: 出错或集合对象的类型错误(non-set)
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
     *    -1: 出错或集合对象的类型错误(non-set)
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
     *    -1: 出错或集合对象的类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sunionstore(const std::string& dest,
                          const std::vector<std::string>& keys);

    /**
     * @description
     *   判断给定的元素member是否是key关联集合的成员
     * @param [IN] key {const std::string&} 集合对象的key
     * @param [IN] member {const std::string&} 给定的元素
     * @return {int} 返回判断结果, 返回值如下:
     *    1: 给定的元素是集合成员
     *    0: 给定的元素不是集合成员或者key不存在
     *   -1: 出错或集合对象的类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int sismember(const std::string& key, const std::string& member);

    /**
     * @description
     *   获取key关联的集合的所有成员. 效果和只有一个key值的sinter函数一样.
     * @param [IN] key {const std::string&} 集合对象的key
     * @param [OUT] result {std::vector<std::string>&} 存放返回结果
     * @return {int} 返回操作结果, 返回值如下:
     *   >0: 返回元素个数
     *    0: key不存在
     *   -1: 出错或keys的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int smembers(const std::string& key, std::vector<std::string>& result);

    /**
     * @description
     *   把给定的元素member从src关联的集合移到dest关联的集合. 该操作是一个原子操
     *   作. 在别的客户端看来, 在给定时刻元素member只存在于src中或dest中.
     *   1) src集合不存在或者不包含元素member, 则不做任何操作并返回0.
     *   2) 当元素member已经存在于dest集合中, 则只把元素member从src集合中移除.
     * @param [IN] src {const std::string&} src集合对象的键值
     * @param [IN] dest {const std::string&} dest集合对象的键值
     * @param [IN] member {const std::string&} 给定需要移动的元素
     * @return {int} 返回判断结果, 返回值如下:
     *    1: 给定的元素被移动
     *    0: 给定的元素不是src集合成员或者src不存在
     *   -1: 出错或者src/dest集合对象的类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int smove(const std::string& src, const std::string& dest,
              const std::string& member);

    /**
     * @description
     *   随机的从key关联的集合中弹出一个元素, 并返回弹出的元素.
     * @param [IN] key {const std::string&} 集合对象的key
     * @param [OUT] result {std::string&} 存放弹出的元素
     * @return {int} 返回判断结果, 返回值如下:
     *    1: 随机弹出一个元素
     *    0: key不存在
     *   -1: 出错或key的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int spop(const std::string& key, std::string& result);

    /**
     * @description
     *   随机的从key关联的集合中弹出count个元素, 并返回弹出的元素.
     *   3.2以上版本支持.
     * @param [IN] key {const std::string&} 集合对象的key
     * @param [OUT] result {std::vector<std::string>&} 存放弹出的元素
     * @param [IN] count {int} 弹出元素的个数, 取值如下:
     *   >0: 弹出count个的元素, 如果count大于集合元素数量, 则弹出所有元素
     *    0: 不弹出, 返回空列表
     * @return {int} 返回判断结果, 返回值如下:
     *   >0: 弹出元素的个数
     *    0: key不存在
     *   -1: count为负值或key的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int spop(const std::string& key,
             std::vector<std::string>& result, int count);

    /**
     * @description
     *   随机的从key关联的集合中返回一个元素. 
     * @param [IN] key {const std::string&} 集合对象的key
     * @param [OUT] result {std::string&} 存放返回的元素
     * @return {int} 返回判断结果, 返回值如下:
     *    1: 随机返回一个元素
     *    0: key不存在
     *   -1: 出错或key的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int srandmember(const std::string& key, std::string& result);

    /**
     * @description
     *   随机的从key关联的集合中返回count个元素. 
     * @param [IN] key {const std::string&} 集合对象的key
     * @param [OUT] result {std::string&} 存放返回的元素
     * @param [IN] count {int} 弹出元素的个数, 取值如下:
     *   >0: 返回count个不重复的元素, 如果count大于集合元素数量, 则返回所有元素
     *    0: 返回空列表
     *   <0: 返回count个元素(可重复)
     * @return {int} 返回判断结果, 返回值如下:
     *   >0: 返回元素的个数
     *    0: key不存在或count=0
     *   -1: 出错或key的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    int srandmember(const std::string& key,
                    std::vector<std::string>& result, int count);

    /**
     * @description
     *   从key关联的集合中移除指定元素. 如果集合中不包含指定元素, 则忽略. 如果
     *   key不存在, 则当成空集合, 并返回0.
     * @param [IN] key {const std::string&} 集合对象的key
     * @param [IN] member {const std::vector<std::string>&} 需要移除的元素列表
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 移除的元素个数, 不包括本来就不存在的元素
     *    0: key不存在或者没有元素被移除
     *   -1: 出错或key的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    long long srem(const std::string& key,
                   const std::vector<std::string>& member);

    /**
     * @description
     *   用于迭代key关联的集合中的元素
     * @param [IN] key {const std::string&} 集合对象的key
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
    int sscan(const std::string& key, int cursor,
              std::vector<std::string>& result,
              const char* pattern = NULL, int count = 10);

private:
    bool set_operation(const char* op, const std::vector<std::string>& keys,
                       std::vector<std::string>& result);
    long long set_operation_with_store(const char * op, const std::string& dest,
                                       const std::vector<std::string>& keys);
};

#endif /* __REDIS_SET_H__ */
