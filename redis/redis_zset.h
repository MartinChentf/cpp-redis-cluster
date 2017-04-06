#ifndef __REDIS_ZSET_H__
#define __REDIS_ZSET_H__

#include <string>
#include <map>

#include "redis_command.h"

class redis_client;

class redis_zset : public redis_command
{
public:
    redis_zset(redis_client* client):redis_command(client){}
    ~redis_zset(){}

public:
    /**
     * @description
     *   向key关联的有序集合中添加所有指定的元素. 每个元素对应一个分数.
     *   1) 如果给定的成员已经存在于有序集合中, 则更新其分数并更新排序位置
     *   2) 如果key不存在, 则新建一个有序集合并将分数/成员添加到有序集合中
     *   3) 如果key的value类型错误(non-sortedset), 则返回一个错误
     *   4) 分数支持双精度64位浮点数
     *   5) 有序集合按照分数递增排序, 如果两个元素分数, 则按照元素的字典顺序排序
     * @param [IN] key {const std::string&} 有序集合对象的key
     * @param [IN] score_mems {const std::map<double, std::string>&} 分数/元素对
     * @param [IN] nx_or_xx {int} 是否使用NX或XX标志, 默认值为0, 取值范围如下:
     *    1: 使用XX标志, 只更新已经存在于有序集合中的元素的分数, 不增加新元素
     *    0: 禁用NX/XX标志
     *   -1: 使用NX标志, 不更新已经存在于有序集合中的元素的分数, 只增加新元素
     * @param [IN] is_change {bool} 是否修改返回值, 默认值为false, 取值如下:
     *    true: 返回值为所有被改变的元素的个数, 包括新加元素和分数更新的元素
     *   false: 返回值为新增加的元素个数
     * @return {int} 返回操作结果, 返回值如下:
     *   >=0: 新增加的元素个数
     *   -1: 出错或key的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    long long zadd(const std::string& key,
                   const std::map<double, std::string>& score_mems,
                   int nx_or_xx = 0, bool is_change = false);

    long long zadd_nx(const std::string& key,
                      const std::map<double, std::string>& score_mems,
                      bool is_change = false);

    long long zadd_xx(const std::string& key,
                      const std::map<double, std::string>& score_mems,
                      bool is_change = false);

    long long zadd_ch(const std::string& key,
                      const std::map<double, std::string>& score_mems,
                      int nx_or_xx = 0);

    /**
     * @description
     *   对key关联的有序集合中指定的元素member的分数值进行累加. 行为和zincrby函
     *   数一样.
     * @param [IN] key {const std::string&} 有序集合对象的key
     * @param [IN] score {double} 输入: 累加的分数, 输出: 累加后的分数
     * @param [IN] member {const std::string&} 需要累加的成员
     * @param [OUT] result {std::string&} 存储累加结果
     * @return {int} 返回操作结果, 返回值如下:
     *    true: 累加成功
     *   false: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    bool zadd_incr(const std::string& key, double score,
                   const std::string& member, std::string& result);

    /**
     * @description
     *   返回key关联的有序集合的基数(集合的元素个数).
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 集合的基数.
     *    0: key不存在.
     *   -1: 出错或key的value类型错误(non-set).
     * @author chen.tengfei
     * @date 2017-04-05
     */
    long long zcard(const std::string& key);

    /**
     * @description
     *   返回key关联的有序集合中, 分数值在[min, max]区间范围内的成员.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {double} 分数区间下限.
     * @pzrzm [IN] max {double} 分数区间上限.
     * @return {long long} 返回指定分数范围的元素个数.
     * @author chen.tengfei
     * @date 2017-04-05
     */
    long long zcount(const std::string& key, double min, double max);

    /**
     * @description
     *   对key关联的有序集合中指定的元素member的分数值累加increment.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] increment {double} 累加值
     * @param [IN] member {const std::string&} 需要累加的成员
     * @param [OUT] result {std::string&} 存储累加结果
     * @return {int} 返回操作结果, 返回值如下:
     *    true: 累加成功
     *   false: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-05
     */
    bool zincrby(const std::string& key, double increment,
                    const std::string& member, std::string& result);

    /**
     * @description
     *   计算给定numkeys个有序集合的交集, 并将计算结果放入dest中.
     * @param [IN] dest {const std::string&} 存放计算结果的有序集合对象
     * @param [IN] numkeys {int} 需要计算的有序集合的个数
     * @param [IN] keys {const std::vector<std::string>&}
     *   给定的一组有序集合. keys的数量由numkeys指定, 否则出错
     * @param [IN] weights {const std::vector<double>&} 有序集合对应的权重列表.
     *   每一个集合对应一个权重值. 当权重列表为空时, 表示默认权重值为1.
     * @param [IN] aggregate {const char*} 指定结果集的聚合方式. 默认值为"SUM",
     *   取值如下:
     *   "SUM": 将所有集合中某个成员的score值之和作为结果集中该成员的score值
     *   "MIN": 将所有集合中某个成员的score值的最小值作为结果集中该成员的score值
     *   "MAX": 将所有集合中某个成员的score值的最大值作为结果集中该成员的score值
     * @return {long long} 结果集dest中元素的个数, 取值如下:
     *   >=0: 结果集dest中元素的个数
     *    -1: 出错或dest/keys的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-06
     */
    long long zinterstore(const std::string& dest, int numkeys,
                          const std::vector<std::string>& keys,
                          const std::vector<double>& weights,
                          const char* aggregate = "SUM");

    /**
     * @description
     *   计算给定numkeys个有序集合的交集, 并将计算结果放入dest中.
     * @author chen.tengfei
     * @date 2017-04-06
     */
    long long zunionstore(const std::string& dest, int numkeys,
                          const std::vector<std::string>& keys,
                          const std::vector<double>& weights,
                          const char* aggregate = "SUM");

private:
    long long zstore(const char* cmd, const std::string& dest, int numkeys,
                     const std::vector<std::string>& keys,
                     const std::vector<double>& weights,
                     const char* aggregate);
};

#endif /* __REDIS_ZSET_H__ */
