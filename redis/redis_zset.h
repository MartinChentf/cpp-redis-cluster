#ifndef __REDIS_ZSET_H__
#define __REDIS_ZSET_H__

#include <string>
#include <vector>

#include "redis_command.h"

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
     * @param [IN] member_score
     *   {const std::vector<std::pair<std::string, double>>&} 元素/分数对
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
            const std::vector< std::pair<std::string, double> >& member_score,
            int nx_or_xx = 0, bool is_change = false);

    long long zadd_nx(const std::string& key,
            const std::vector< std::pair<std::string, double> >& member_score,
            bool is_change = false);

    long long zadd_xx(const std::string& key,
            const std::vector< std::pair<std::string, double> >& member_score,
            bool is_change = false);

    long long zadd_ch(const std::string& key,
            const std::vector< std::pair<std::string, double> >& member_score,
            int nx_or_xx = 0);

    /**
     * @description
     *   对key关联的有序集合中指定的元素member的分数值进行累加. 行为和zincrby函
     *   数一样.
     * @param [IN] key {const std::string&} 有序集合对象的key
     * @param [IN] score {double} 累加的分数
     * @param [IN] member {const std::string&} 需要累加的成员
     * @param [OUT] result {std::string&} 存储累加结果
     * @return {bool} 返回操作结果, 返回值如下:
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
     *   返回key关联的有序集合中, 分数值在[min, max](闭区间)范围内的元素个数.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {double} 分数区间下限.
     * @pzrzm [IN] max {double} 分数区间上限.
     * @return {long long} 返回指定分数范围的元素个数, 返回值如下:
     *   >=0: 指定分数范围的元素个数
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-05
     */
    long long zcount(const std::string& key, double min, double max);

    /**
     * @description
     *   返回key关联的有序集合中, 分数值在[min, max](闭区间)范围内的元素个数.
     *   1) min和max可以使用-inf和+inf. 这样可以在不知道有序集合最低和最高分数值
     *      的情况下使用本函数.
     *   2) 默认情况下, 分数区间的取值使用闭区间, 可以在分数值前面加上'('符号来
     *      使用开区间.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {const std::string&} 分数区间下限.
     * @pzrzm [IN] max {const std::string&} 分数区间上限.
     * @return {long long} 返回指定分数范围的元素个数, 返回值如下:
     *   >=0: 指定分数范围的元素个数
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @param [IN/OUT] name {type} 
     * @return {type} 
     * @author chen.tengfei
     * @date 2017-04-15
     */
    long long zcount(const std::string& key, const std::string& min,
                     const std::string& max);

    /**
     * @description
     *   对key关联的有序集合中指定的元素member的分数值累加increment.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] increment {double} 累加值
     * @param [IN] member {const std::string&} 需要累加的成员
     * @param [OUT] result {std::string*} 不为空时, 存储累加结果
     * @return {int} 返回操作结果, 返回值如下:
     *    true: 累加成功
     *   false: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-05
     */
    bool zincrby(const std::string& key, double increment,
                    const std::string& member, std::string* result = NULL);

    /**
     * @description
     *   计算给定numkeys个有序集合的交集, 并将计算结果放入dest中.
     * @param [IN] dest {const std::string&} 存放计算结果的有序集合对象
     * @param [IN] keys {const std::vector<std::string>&}
     *   给定的一组有序集合/集合(所有集合成员的分值都被视为1).
     * @param [IN] weights {const std::vector<double>*} 有序集合对应的权重列表.
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
    long long zinterstore(const std::string& dest,
                          const std::vector<std::string>& keys,
                          const std::vector<double>* weights = NULL,
                          const char* aggregate = "SUM");

    /**
     * @description
     *   计算给定numkeys个有序集合的交集, 并将计算结果放入dest中.
     * @author chen.tengfei
     * @date 2017-04-06
     */
    long long zunionstore(const std::string& dest,
                          const std::vector<std::string>& keys,
                          const std::vector<double>* weights = NULL,
                          const char* aggregate = "SUM");

    /**
     * @description
     *   当有序集合的所有成员具有相同分数值时, 有序集合的元素会根据成员的字典序
	 *   (lexicographical ordering)来进行排序, 而这个命令则可以返回key关联的有序
	 *   集合中, 值介于元素min和元素max之间的成员数量. min和max的用法参考
	 *   zrangebylex函数.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {const std::string&} 区间元素最小值
     * @param [IN] max {const std::string&} 区间元素最大值
     * @return {long long} 返回指定区间内的成员数量, 返回值如下:
     *   >=0: 指定区间内的成员数量
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    long long zlexcount(const std::string& key, const std::string& min,
                        const std::string& max);

    /**
     * @description
     *   返回key关联的有序集合中[start, stop](闭区间)指定范围内的元素. 所有成
     *   员按照分数值升序排序, 分数相同的元素按照字典序升序排序. start和stop是基
     *   于0的下标, 可为负数, -1表示最后一个元素, -2表示倒数第二个元素. 如果
     *   start大于列表末尾下标或者start > stop, 返回结果将为空列表. 如果stop大于
     *   列表下标, 则会被当做列表末尾下标.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] start {int} 起始下标
     * @param [IN] stop {int} 结束下标
     * @param [OUT] result {std::vector<std::string>&} 存储返回的结果集
     * @return {int} 返回结果集大小, 返回值如下:
     *   >=0: 结果集大小
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrange(const std::string& key, int start, int stop,
               std::vector<std::string>& result);

    /**
     * @description
     *   返回key关联的有序集合中[start, stop](闭区间)指定范围内的元素及其分数.
     *   start/stop取值范围参考zrange函数.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] start {int} 起始下标
     * @param [IN] stop {int} 结束下标
     * @param [OUT] result {std::vector<std::pair<std::string, double>>&}
     *   存储返回的结果集
     * @return {int} 返回结果集大小, 返回值如下:
     *   >=0: 结果集大小
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrange_with_scores(const std::string& key, int start, int stop,
        std::vector< std::pair<std::string, double> >& result);

    /**
     * @description
     *   当有序集合的所有成员具有相同分数值时, 有序集合的元素会根据成员的字典序
	 *   (lexicographical ordering)来进行排序, 而这个命令则可以返回key关联的有序
	 *   集合中, 值介于元素min和元素max之间的成员.
	 *   1) 如果有序集合中元素的分数不一样, 则返回结果元素将不准确.
	 *   2) 字符串按照二进制字节数组进行比较, 从小到大排序. 如果字符串中使用了非
	 *      ASCII字符集(比如UTF-8), 则会影响返回结果
	 *   3) 有效的min和max必须以'('或'['开头, 表示包含或不包含min/max元素.
	 *   4) min和max位置可以使用'-'或'+'表示最小值和最大值.
	 *      e.g. zrangebylex("foo", "-", "+", out)
	 *           将会返回有序集合foo中所有的元素
	 *   5) max和min的位置不能交换, 否则将会返回空集
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {const std::string&} 区间元素最小值
     * @param [IN] max {const std::string&} 区间元素最大值
     * @param [OUT] result {std::vector<std::string>&} 存储结果集
     * @param [IN] offset {int} 从结果集中选取的下标起始值, 取值范围:
     *   >=0: 结果集中的下标值
     *    -1: 结果集不分片, 默认值
     * @paran [IN] count {int} 从结果集offset指定的位置开始选取的元素数量
     *   >0: 为有效值
     *    0: 默认值, 当offset < 0时, 参数offset和count不起作用
     * @return {int} 返回结果集大小, 返回值如下:
     *   >=0: 结果集大小
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebylex(const std::string& key, const std::string& min,
                    const std::string& max, std::vector<std::string>& result,
                    int offset = -1, int count = 0);

    /**
     * @description
     *   返回key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {double} 分数区间下限.
     * @pzrzm [IN] max {double} 分数区间上限.
     * @param [OUT] result {std::vector<std::string>&} 存储结果集
     * @param [IN] offset {int} 从结果集中选取的下标起始值, 取值范围:
     *   >=0: 结果集中的下标值
     *    -1: 结果集不分片, 默认值
     * @paran [IN] count {int} 从结果集offset指定的位置开始选取的元素数量
     *   >0: 为有效值
     *    0: 默认值, 当offset < 0时, 参数offset和count不起作用
     * @return {int} 返回结果集大小, 返回值如下:
     *   >=0: 结果集大小
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore(const std::string& key, double min, double max,
                      std::vector<std::string>& result,
                      int offset = -1, int count = 0);

    /**
     * @description
     *   返回key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素
     *   1) min和max可以使用-inf和+inf. 这样可以在不知道有序集合最低和最高分数值
     *      的情况下使用本函数.
     *   2) 默认情况下, 分数区间的取值使用闭区间, 可以在分数值前面加上'('符号来
     *      使用开区间.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {const std::string&} 分数区间下限.
     * @pzrzm [IN] max {const std::string&} 分数区间上限.
     * @param [OUT] result {std::vector<std::string>&} 存储结果集
     * @param [IN] offset {int} 从结果集中选取的下标起始值(基于0), 取值范围:
     *   >=0: 结果集中的下标值
     *    -1: 结果集不分片, 默认值
     * @paran [IN] count {int} 从结果集offset指定的位置开始选取的元素数量
     *   >0: 为有效值
     *    0: 默认值, 当offset < 0时, 参数offset和count不起作用
     * @return {int} 返回结果集大小, 返回值如下:
     *   >=0: 结果集大小
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore(const std::string& key, const std::string& min,
                      const std::string& max, std::vector<std::string>& result,
                      int offset = -1, int count = 0);

    /**
     * @description
     *   返回key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素及其
     *   分数
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {double} 分数区间下限.
     * @pzrzm [IN] max {double} 分数区间上限.
     * @param [OUT] result {std::vector<std::pair<std::string, double>>&}
     *   存储结果集
     * @param [IN] offset {int} 从结果集中选取的下标起始值, 取值范围:
     *   >=0: 结果集中的下标值
     *    -1: 结果集不分片, 默认值
     * @paran [IN] count {int} 从结果集offset指定的位置开始选取的元素数量
     *   >0: 为有效值
     *    0: 默认值, 当offset < 0时, 参数offset和count不起作用
     * @return {int} 返回结果集大小, 返回值如下:
     *   >=0: 结果集大小
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore_with_scores(const std::string& key,
        double min, double max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   返回key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素及其
     *   分数
     *   1) min和max可以使用-inf和+inf. 这样可以在不知道有序集合最低和最高分数值
     *      的情况下使用本函数.
     *   2) 默认情况下, 分数区间的取值使用闭区间, 可以在分数值前面加上'('符号来
     *      使用开区间.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {const std::string&} 分数区间下限.
     * @pzrzm [IN] max {const std::string&} 分数区间上限.
     * @param [OUT] result {std::vector<std::pair<std::string, double>>&}
     *   存储结果集
     * @param [IN] offset {int} 从结果集中选取的下标起始值, 取值范围:
     *   >=0: 结果集中的下标值
     *    -1: 结果集不分片, 默认值
     * @paran [IN] count {int} 从结果集offset指定的位置开始选取的元素数量
     *   >0: 为有效值
     *    0: 默认值, 当offset < 0时, 参数offset和count不起作用
     * @return {int} 返回结果集大小, 返回值如下:
     *   >=0: 结果集大小
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrangebyscore_with_scores(const std::string& key,
        const std::string& min, const std::string& max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   返回元素member在key关联的有序集合中的排名, 有序集合按照分数值递增排列.
     *   该排名(或下标)是基于0的, 也就是说, score值最小的成员排名为0.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] member {const std::string&} 指定的元素member.
     * @return {int} 返回指定元素在有序集合中的排名, 返回值如下:
     *   >=0: 指定元素在有序集合中的排名
     *    -1: 1) 出错或key的value类型错误(non-sortedset)
     *        2) key不存在或者member不存在
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrank(const std::string& key, const std::string& member);

    /**
     * @description
     *   从key关联的有序集合中移除指定元素. 如果集合中不包含指定元素, 则忽略. 如
     *   果key不存在, 则当成空集合, 并返回0.
     * @param [IN] key {const std::string&} 集合对象的key
     * @param [IN] member {const std::vector<std::string>&} 需要移除的元素列表
     * @return {long long} 返回操作结果, 返回值如下:
     *   >0: 移除的元素个数, 不包括本来就不存在的元素
     *    0: key不存在或者没有元素被移除
     *   -1: 出错或key的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    long long zrem(const std::string& key,
                   const std::vector<std::string>& member);

    /**
     * @description
     *   当有序集合的所有成员具有相同分数值时, 有序集合的元素会根据成员的字典序
	 *   (lexicographical ordering)来进行排序, 而这个命令则可以移除key关联的有序
	 *   集合中, 值介于元素min和元素max之间的成员.
	 *   min和max的含义请参考zrangebylex函数.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {const std::string&} 区间元素最小值
     * @param [IN] max {const std::string&} 区间元素最大值
     * @return {int} 返回移除元素的数量, 返回值如下:
     *   >=0: 移除元素的数量
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebylex(const std::string& key, const std::string& min,
                       const std::string& max);

    /**
     * @description
     *   移除key关联的有序集合中[start, stop](闭区间)指定范围内的元素.
     *   start和stop的含义请参考zrange函数.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] start {int} 起始下标
     * @param [IN] stop {int} 结束下标
     * @return {int} 返回移除元素的数量, 返回值如下:
     *   >=0: 移除元素的数量
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebyrank(const std::string& key, int start, int stop);

    /**
     * @description
     *   移除key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {double} 分数区间下限.
     * @pzrzm [IN] max {double} 分数区间上限.
     * @return {int} 返回移除元素的数量, 返回值如下:
     *   >=0: 移除元素的数量
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebyscore(const std::string& key, double min, double max);

    /**
     * @description
     *   移除key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素
     *   1) min和max可以使用-inf和+inf. 这样可以在不知道有序集合最低和最高分数值
     *      的情况下使用本函数.
     *   2) 默认情况下, 分数区间的取值使用闭区间, 可以在分数值前面加上'('符号来
     *      使用开区间.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] min {const std::string&} 分数区间下限.
     * @pzrzm [IN] max {const std::string&} 分数区间上限.
     * @return {int} 返回移除元素的数量, 返回值如下:
     *   >=0: 移除元素的数量
     *    -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zremrangebyscore(const std::string& key, const std::string& min,
                         const std::string& max);

    /**
     * @description
     *   返回key关联的有序集合中[start, stop](闭区间)指定范围内的元素. 除了逆序
     *   之外, 参数和返回值同zrange函数一样.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrange(const std::string& key, int start, int stop,
                  std::vector<std::string>& result);

    /**
     * @description
     *   返回key关联的有序集合中[start, stop](闭区间)指定范围内的元素及其分数.
     *   除了逆序之外, 参数和返回值同zrange_with_scores函数一样.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrange_with_scores(const std::string& key,
        int start, int stop,
        std::vector< std::pair<std::string, double> >& result);

    /**
     * @description
     *   当有序集合的所有成员具有相同分数值时, 有序集合的元素会根据成员的字典序
	 *   (lexicographical ordering)来进行排序, 而这个命令则可以返回key关联的有序
	 *   集合中, 值介于元素min和元素max之间的成员.
     *   除了逆序之外, 参数和返回值同zrangebylex函数一样.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebylex(const std::string& key, const std::string& min,
        const std::string& max, std::vector<std::string>& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   返回key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素.
     *   除了逆序之外, 参数和返回值同zrangebyscore函数一样.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore(const std::string& key, double min, double max,
        std::vector<std::string>& result, int offset = -1, int count = 0);

    /**
     * @description
     *   返回key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素.
     *   除了逆序之外, 参数和返回值同zrangebyscore函数一样.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore(const std::string& key, const std::string& min,
        const std::string& max, std::vector<std::string>& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   返回key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素及其分
     *   数. 除了逆序之外, 参数和返回值同zrangebyscore_with_scores函数一样.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore_with_scores(const std::string& key,
        double min, double max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   返回key关联的有序集合中分数值在[min, max](闭区间)范围内的所有元素及其分
     *   数. 除了逆序之外, 参数和返回值同zrangebyscore_with_scores函数一样.
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrangebyscore_with_scores(const std::string& key,
        const std::string& min, const std::string& max,
        std::vector< std::pair<std::string, double> >& result,
        int offset = -1, int count = 0);

    /**
     * @description
     *   返回元素member在key关联的有序集合中的排名, 有序集合按照分数值递减排列.
     *   该排名(或下标)是基于0的, 也就是说, score值最大的成员排名为0.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] member {const std::string&} 指定的元素member.
     * @return {int} 返回指定元素在有序集合中的排名, 返回值如下:
     *   >=0: 指定元素在有序集合中的排名
     *    -1: 1) 出错或key的value类型错误(non-sortedset)
     *        2) key不存在或者member不存在
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zrevrank(const std::string& key, const std::string& member);

    /**
     * @description
     *   用于迭代key关联的有序集合中的键值对
     * @param [IN] key {const std::string&} 有序集合对象的key
     * @param [IN] cursor {int} 游标值, 第一次迭代使用0作为游标.
     * @param [OUT] result {std::vector< std::pair<std::string, double> >&}
     *   存储结果集, 内部以追加方式将本次遍历结果添加进该对象中, 为防止因总结果
     *   集过大导致该数组溢出,用户可在调用本函数前后清理该对象.
     * @param [IN] pattern {const char*} glob风格的模式参数, 非空时有效
     * @param [IN] count {int} 限定结果集元素的数量, 默认值:10
     * @return {int} 下一个游标位置, 返回值如下:
     *   >0: 下一个游标位置
     *    0: 遍历结束
     *   -1: key的value类型错误(non-hash)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zscan(const std::string& key, int cursor,
              std::vector< std::pair<std::string, double> >& result,
              const char* pattern = NULL, int count = 10);

    /**
     * @description
     *   返回key关联的有序集合中元素member的分数.
     * @param [IN] key {const std::string&} 有序集合对象的key.
     * @param [IN] member {const std::string&} 指定的元素member.
     * @param [OUT] soube {std::string&} 存储指定元素的分数
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 成功返回指定元素的分数
     *    0: key不存在或者member不存在
     *   -1: 出错或key的value类型错误(non-sortedset)
     * @author chen.tengfei
     * @date 2017-04-08
     */
    int zscore(const std::string& key, const std::string& member,
               std::string& score);

private:
    long long zstore(const char* cmd, const std::string& dest,
                     const std::vector<std::string>& keys,
                     const std::vector<double>* weights,
                     const char* aggregate);

    int zrangeby(const char* cmd, const std::string& key,
                 const std::string& min, const std::string& max,
                 std::vector<std::string>& result,
                 int offset, int count, bool with_scores = false);

    // 把元素/分数组成的列表转换成元素/分数键值对输出, 并返回键值对个数
    // 如果列表中元素不是成对出现, 则返回-1
    int parse_element_score(const std::vector<std::string>& in,
                            std::vector< std::pair<std::string, double> >& out);
};

#endif /* __REDIS_ZSET_H__ */
