#ifndef __REDIS_ZSET_H__
#define __REDIS_ZSET_H__

#include "redis_commamd.h"

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
     *   >0: 随机返回一个元素
     *    0: key不存在
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
                      const char* nx_or_xx = NULL);

    /**
     * @description
     *   对key关联的有序集合中指定的元素member的分数值进行累加. 行为和zincrby函
     *   数一样
     * @param [IN] key {const std::string&} 有序集合对象的key
     * @param [IN/OUT] score {double} 输入: 累加的分数, 输出: 累加后的分数
     * @param [IN] member {const std::string&} 需要累加的成员
     * @return {int} 返回操作结果, 返回值如下:
     *    true: 累加成功
     *   false: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-03-31
     */
    bool zadd_incr(const std::string& key, double& score,
                        const std::string& member);

    long long zcard(const std::string& key);
    long long zcount(const std::string& key, double min, double max);
    long long zcount(const std::string& key, std::string min, std::string max);
    double zincrby(const std::string& key, double increment, const std::string& member);
};

#endif /* __REDIS_ZSET_H__ */
