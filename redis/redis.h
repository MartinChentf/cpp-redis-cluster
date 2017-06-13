/**
 * Copyright 2017-2022 chen.tengfei <chentf1836@163.com>
 * All rights reserved.
 *
 * @file   redis.h
 * @brief
 * @version 1.0
 * @bug none
 * @warning none
 * @other none
 * @author chen.tengfei
 * @data   2017-05-10 16:55:19
 */
#ifndef __REDIS_H__
#define __REDIS_H__

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

#include "RedisClient.h"

class redis
{
public:
    redis(const std::string host, uint16_t port);
    ~redis();

public:
    /***************************************************************************
     * string command
     ***************************************************************************/
    /**
     * @description
     *   返回key所关联的字符串的值
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [OUT] result {std::string&}
     *   返回key所关联的字符串值, 如果key不存在返回空字符串. 此时需要根据返回值
     *   判断是否key是否存在
     * @return {int} 返回操作结果, 返回值如下:
     *   1: key存在, 并成功返回所关联的字符串值
     *   0: key不存在, 返回空字符串
     *  -1: 出错或key非字符串对象
     */
    int get(const std::string& key, std::string& result);

    /**
     * @description
     *   将key关联的字符串值设置为value，并返回key所关联的旧值
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串新值
     * @param [OUT] result {std::string&} key所关联的字符串旧值，如果key不存在返
     *   回空字符串. 此时需要根据返回值判断是否key是否存在
     * @return {int} 返回操作结果, 返回值如下:
     *   1: key存在, 并成功返回所关联的字符串值
     *   0: key不存在, 返回空字符串
     *  -1: 出错或key非字符串对象
     */
    int getSet(const std::string& key, const std::string& value,
        std::string& result);

    /**
     * @description
     *   将key关联到字符串value, 如果key存在, 将覆盖原来的值,
     *   并且忽略原始类型
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功, 该操作不会失败
     */
    bool set(const std::string& key, const std::string& value);

    /**
     * @description
     *   如果key不存在，将key关联到字符串value
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功, 该操作不会失败. 返回值如下:
     *    true: key被设置
     *   false: key未被设置
     */
    bool setnx(const std::string& key, const std::string& value);

    /**
     * @description
     *   如果key存在，将key关联到字符串value, 行为与 setnx 相反
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功, 该操作不会失败. 返回值如下:
     *    true: key被设置
     *   false: key未被设置
     */
    bool setxx(const std::string& key, const std::string& value);

    /**
     * @description
     *   将key关联到字符串value, 并且设置key在指定的时间second之后超时
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] second {long long} 超时时间, 单位:秒
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功, 该操作不会失败
     */
    bool setex(const std::string& key, long long second,
        const std::string& value);

    /**
     * @description
     *   将key关联到字符串value, 并且设置key在指定的时间millisecond之后超时
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] second {long long} 超时时间, 单位:毫秒
     * @param [IN] value {const std::string&} key所关联的字符串值
     * @return {bool} 操作是否成功, 该操作不会失败
     */
    bool psetex(const std::string& key, long long millisecond,
        const std::string& value);

    /**
     * @description
     *   返回key对应的字符串的子串, 子串由位移量start和end决定(两者都在子串内)
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] start {int} 子串起始位移
     * @param [IN] end {int} 子串末尾位移
     * @param [OUT] result {std::string&} 返回指定的子串
     * @return {bool} 操作是否成功, false表示出错或key非字符串对象
     */
    bool getrange(const std::string& key, int start, int end,
        std::string& result);

    /**
     * @description
     *   覆盖key所对应的字符串的部分值, 从指定的offset处开始, 覆盖value的长度.
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] offset {int} 指定偏移量的起始位置, 如果偏移量大于字符串长度,
     *  超出部分用0填充
     * @param [IN] value {const std::string&} 覆盖的值
     * @param [OUT] length {long long *} 返回覆盖后字符串的长度
     * @return {bool} 操作是否成功, false表示出错或key非字符串对象
     */
    bool setrange(const std::string& key, int offset,
        const std::string& value, long long* length = NULL);

    /**
     * @description
     *   返回key所对应的字符串在offset处的bit值. 如果偏移量超出字符串长度, 则超
     *   出部分被假定为由0bits填充的连续空间. 当key不存在的时候, 它就认为是一个
     *   空字符串, 所以offset总是超出范围, 则字符串被假定为由0bits填充的连续空间.
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] offset {int} 指定bit位的偏移量.
     *   第一个字节的最高有效位为0, 第二字节最高有效位为8, 依此类推.
     *   setbit和bitpos的bit位偏移量与此相似.
     * @return {int} 返回指定比特位的值, 返回值如下:
     *   0或1: 正常返回
     *   -1: 出错
     */
    int getbit(const std::string& key, int offset);

    /**
     * @description
     *   设置或清除key所对应的字符串在offset处的bit位. 如果偏移量超出字符串长度,
     *   该字符串就被假定为由0bits填充的连续空间. 当key不存在的时候, 它就认为是
     *   一个空字符串, 所以offset总是超出范围, 则字符串被假定为由0bits填充的连续
     *   空间.
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] offset {int} 指定bit位的偏移量. offset范围: [0, 2^32)
     * @param [IN] value {bool} 设置或清除offset处的bit位
     *   true: 设置offset处bit位
     *   false: 清除offset处bit位
     * @return {int} 返回offset处原比特值, 返回值如下:
     *   0或1: 正常返回
     *   -1: 出错
     */
    int setbit(const std::string& key, int offset, bool value);

    /**
     * @description
     *   统计key所对应的字符串被置为1的bit位数, 若指定start和end, 则只统计
     *   [start, end]区间内置1的bit位数
     * @param [IN] key {const std::string&} 字符串对象的key
     * @param [IN] start {int} 起始字节的偏移量
     * @param [IN] end {int} 末尾字节的偏移量
     * @return {long long} 被置为1的bit位数
     *  >=0: 被置为1的bit位数
     *   -1: 出错
     */
    long long bitcount(const std::string& key);
    long long bitcount(const std::string& key, int start, int end);

    /**
     * @description
     *   对一个或多个保存二进制位的字符串src_keys进行位元操作, 并将结果保存到
     *   dest_key中
     * @param [IN] op {BITOP}
     *   位操作符, 只能使用枚举值 enum BITOP{ AND, OR, NOT, XOR}
     *   AND: 对一个或多个key值求逻辑与, 并将结果保存到dest_key中
     *    OR: 对一个或多个key值求逻辑或, 并将结果保存到dest_key中
     *   NOT: 对key值逻辑取反,  并将结果保存到dest_key中
     *   XOR: 对一个或多个key值求逻辑异或,  并将结果保存到dest_key中
     * @param [OUT] dest_key {const std::string&}
     *   保存结果的字符串对象
     * @param [IN] src_keys {const std::vector<std::string>&}
     *   需要计算的一个或一组字符串对象
     * @return {long long} 返回保存到dest_key的字节长度
     *   >=0: 保存到dest_key的字节长度
     *    -1: 出错
     */
    long long bitop(BITOP op, const std::string& dest_key,
        const std::vector<std::string>& src_keys);
    long long bitop(BITOP op, const std::string& dest_key,
        const std::string& src_key); // 单src_key的版本

    /**
     * @description
     *   返回字符串里面第一个被设置为value(0或1)的bit位, 如果指定start或end,
     *   则在[start, end]区间范围内检索, start和end是指字符串中字节的偏移量,
     *   不是bit位的偏移量
     * @param [IN] key {const std::string&} 被检索的字符串对象的key
     * @param [IN] value {bool} 被检索的bit值
     *   false: 检索bit值为0的位
     *    true: 检索bit值为1的位
     * @param [IN] start {int}
     *   检索的起始字节的偏移量, 默认值为0, 表示从头开始检索
     * @param [IN] end {int}
     *   检索的末尾字节的偏移量, 默认值为-1, 表示一直检索到字符串末尾
     * @return {long long} 返回字符串里面第一个被设置为value(0或1)的bit位
     *   >=0: 第一个被设置为value(0或1)的bit位
     *    -1: 出错
     */
    long long bitpos(const std::string& key, bool value,
        int start = 0, int end = -1);

    /**
     * @description
     *   返回所有指定的key所关联的value, 如果key所关联的值不是string类型
     *   或key值不存在对应位置为空指针.
     * @param [IN] vkeys {const std::vector<std::string>&}
     *   一组字符串对象的key
     * @param [OUT] result {std::vector<std::string*>&}
     *   keys所关联的value, 需要调用者手动释放内存
     * @return {bool} 返回true表示执行成功, 该调用不会失败
     */
    bool mget(const std::vector<std::string>& vkeys,
        std::vector<std::string*>& result);

    /**
     * @description
     *   把key关联到对应的value上, 会覆盖原有的value值, 行为和set类似.
     * @param [IN] keyValues {const std::map<std::string, std::string>&} 键值对
     * @return {bool} 返回true表示执行成功, 该调用不会失败
     */
    bool mset(const std::map<std::string, std::string>& key_values);

    /**
     * @description
     *   把key关联到对应的value上, 只要有一个key存在, 整个msetnx都不会执行.
     * @param [IN] keyValues {const std::map<std::string, std::string>&} 键值对
     * @return {bool} 返回操作是否成功, 该调用不会失败
     *    true: 表示所有的key被设置成功
     *   false: 没有key被设置(已经有存在的key)
     */
    bool msetnx(const std::map<std::string, std::string>& key_values);

    /**
     * @description
     *   对存储在指定key的数值执行加1操作. 如果key不存在, 自增前将key的值设为0
     * @param [IN] key {const std::string&} 指定自增的key
     * @param [OUT] result {long long*} 非空时存储自增结果
     * @return {bool} 表示自增操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: 1) key的value类型错误(non-string)
     *          2) key的value不能表示成数字的字符串
     */
    bool incr(const std::string& key, long long* result = NULL);

    /**
     * @description
     *   对存储在指定key的数值增加increment.
     *   如果key不存在, 自增前将key的值设为0
     * @param [IN] key {const std::string&} 指定自增的key
     * @param [IN] increment {long long} 增量值, <0表示负增长
     * @param [OUT] result {long long*} 非空时存储自增结果
     * @return {bool} 表示自增操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: 1) key的value类型错误(non-string)
     *          2) key的value不能表示成数字的字符串
     */
    bool incrby(const std::string& key, long long increment,
        long long* result = NULL);

    /**
     * @description
     *   对存储在指定key的浮点数数值增加increment.
     *   如果key不存在, 自增前将key的值设为0
     * @param [IN] key {const std::string&} 指定自增的key
     * @param [IN] increment {double} 增量值, <0表示负增长
     * @param [OUT] result {long long*} 非空时存储自增结果
     * @return {bool} 表示自增操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: 1) key的value类型错误(non-string)
     *          2) key的value不能解析成双精度浮点值的字符串
     */
    bool incrbyfloat(const std::string& key, double increment,
        std::string* result = NULL);

    /**
     * @description
     *   对存储在指定key的数值执行减1操作. 如果key不存在, 自减前将key的值设为0
     * @param [IN] key {const std::string&} 指定自减的key
     * @param [OUT] result {long long*} 非空时存储自减结果
     * @return {bool} 表示自减操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: 1) key的value类型错误(non-string)
     *          2) key的value不能表示成数字的字符串
     */
    bool decr(const std::string& key, long long* result = NULL);

    /**
     * @description
     *   对存储在指定key的数值减去increment. 如果key不存在, 自减前将key的值设为0
     * @param [IN] key {const std::string&} 指定自减的key
     * @param [IN] increment {long long} 增量值, <0表示增加
     * @param [OUT] result {long long*} 非空时存储自减结果
     * @return {bool} 表示自减操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: 1) key的value类型错误(non-string)
     *          2) key的value不能表示成数字的字符串
     */
    bool decrby(const std::string& key, long long decrement,
        long long* result = NULL);

    /**
     * @description
     *   如果key已经存在, 并且值为字符串, 则把value追加到原来值的结尾.
     *   如果key不存在, 那么它将首先创建一个空字符串的key, 再执行追加操作
     * @param [IN] key {const std::string&} 指定追加字符串的key
     * @param [IN] value {const std::string&} 追加的字符串值
     * @param [OUT] length {long long*} 非空时存储追加后字符串的长度
     * @return {bool} 表示操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: key的value类型错误(non-string)
     */
    bool append(const std::string& key, const std::string& value,
        long long* length = NULL);

    /**
     * @description
     *   返回key关联的string的长度, 如果key不存在, 返回0
     * @param [IN] key {const std::string&} 指定字符串的key
     * @param [OUT] length {long long&} 返回字符串的长度
     * @return {bool} 操作是否成功, 返回值如下:
     *    true: 操作成功
     *   false: key的value类型错误(non-string)
     */
    bool strlen(const std::string& key, long long& length);

public:
    /***************************************************************************
     * list command
     ***************************************************************************/
    /**
     * @description
     *   blpop函数是阻塞式列表的弹出原语. 它是lpop函数的阻塞版本.
     *   1) 当给定的所有列表没有任何元素的时候, 连接将被blpop函数阻塞, 直到别的
     *      客户端向这些列表中的任意一个列表push元素或者超时. 当timeout为0时, 表
     *      示无限期阻塞
     *   2) 如果给定的列表中至少有一个非空列表, 则弹出第一个非空列表的头部元素.
     * @param [IN] vkeys {const std::vector<std::string>&} 给定弹出元素的列表组
     * @param [IN] timeout {long long} 超时时间
     * @param [OUT] key {std::string&} 弹出元素的列表
     * @param [OUT] value {std::string&} 弹出元素
     *   存储弹出元素的key以及对应的元素
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 操作成功
     *    0: 超时
     *   -1: 1) vkeys的value类型错误(non-list)
     *       2) 其他错误
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int blpop(const std::vector<std::string>& vkeys, long long timeout,
        std::string& key, std::string& value);

    /**
     * @description
     *   brpop函数是阻塞式列表的弹出原语. 它是rpop函数的阻塞版本. 具体行为与
     *   blpop函数类似, 除了brpop函数是从列表尾部弹出元素.
     * @param [IN] vkeys {const std::vector<std::string>&} 给定弹出元素的列表组
     * @param [IN] timeout {long long} 超时时间
     * @param [OUT] key {std::string&} 弹出元素的列表
     * @param [OUT] value {std::string&} 弹出元素
     *   存储弹出元素的key以及对应的元素
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 操作成功
     *    0: 超时
     *   -1: 1) keys的value类型错误(non-list)
     *       2) 其他错误
     * @author chen.tengfei
     * @date 2017-03-29
     */
    int brpop(const std::vector<std::string>& vkeys, long long timeout,
        std::string& key, std::string& value);

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
     *    0: key不存在
     *   -1: 出错或key的value类型错误(non-list)
     */
    long long lpushx(const std::string& key, const std::string& value);

    /**
     * @description
     *   返回key关联的列表中[start, end](闭区间)范围内的元素. start和end为基于0
     *   的下标, 可为负数, -1表示最后一个元素, -2表示倒数第二个元素. 如果start大
     *   于列表末尾下标或者start > stop, 结果将为空列表. 如果stop大于列表下标,则
     *   会被当做列表末尾下标.
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
     * @param [IN] value {const std::string&} 新的元素值
     * @return {bool} 返回操作结果, 返回值如下:
     *    true: 设置新值成功
     *   false: 1) key的value类型错误(non-list)
     *          2) index越界
     *          3) key不存在
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

public:
    /***************************************************************************
     * hash command
     ***************************************************************************/
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
     * @return {int} 返回操作结果, 返回值如下:
     *   >0: 哈希集中域的个数
     *    0: key不存在
     *   -1: 出错或key的value类型错误(non-hash)
     */
    int hgetall(const std::string& key,
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

public:
    /***************************************************************************
     * set command
     ***************************************************************************/
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
     * @param [IN] vkeys {const std::vector<std::string>&} 指定的一组集合
     * @param [OUT] result {std::vector<std::string>&} 存储返回的差集
     * @return {bool} 返回操作结果, false表示出错或keys的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sdiff(const std::vector<std::string>& vkeys,
        std::vector<std::string>& result);

    /**
     * @description
     *   返回所有给定集合的交集. 不存在的key被当做空集.
     * @param [IN] vkeys {const std::vector<std::string>&} 指定的一组集合
     * @param [OUT] result {std::vector<std::string>&} 存储返回的交集
     * @return {bool} 返回操作结果, false表示出错或keys的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sinter(const std::vector<std::string>& vkeys,
        std::vector<std::string>& result);

    /**
     * @description
     *   返回所有给定集合的并集. 不存在的key被当做空集.
     * @param [IN] vkeys {const std::vector<std::string>&} 指定的一组集合
     * @param [OUT] result {std::vector<std::string>&} 存储返回的并集
     * @return {bool} 返回操作结果, false表示出错或keys的value类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    bool sunion(const std::vector<std::string>& vkeys,
        std::vector<std::string>& result);

    /**
     * @description
     *   返回指定的一组集合中第一个集合与其余集合的差集, 并把结果存储在集合dest
     *   中. 如果dest集合已存在, 则覆盖原有元素. 不存在的key被当做空集.
     * @param [IN] dest {const std::string&} 存放计算结果的集合对象的key
     * @param [IN] vkeys {const std::vector<std::string>&} 指定的一组集合
     * @return {long long} 返回结果集的元素个数, 返回值如下:
     *   >=0: 结果集的元素个数
     *    -1: 出错或集合对象的类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sdiffstore(const std::string& dest,
        const std::vector<std::string>& vkeys);

    /**
     * @description
     *   返回所有给定集合的交集, 并把结果存储在集合dest中. 如果dest集合已存在,
     *   则覆盖原有元素. 不存在的key被当做空集.
     * @param [IN] dest {const std::string&} 存放计算结果的集合对象的key
     * @param [IN] vkeys {const std::vector<std::string>&} 指定的一组集合
     * @return {long long} 返回结果集的元素个数, 返回值如下:
     *   >=0: 结果集的元素个数
     *    -1: 出错或集合对象的类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sinterstore(const std::string& dest,
        const std::vector<std::string>& vkeys);

    /**
     * @description
     *   返回所有给定集合的并集, 并把结果存储在集合dest中. 如果dest集合已存在,
     *   则覆盖原有元素. 不存在的key被当做空集.
     * @param [IN] dest {const std::string&} 存放计算结果的集合对象的key
     * @param [IN] vkeys {const std::vector<std::string>&} 指定的一组集合
     * @return {long long} 返回结果集的元素个数, 返回值如下:
     *   >=0: 结果集的元素个数
     *    -1: 出错或集合对象的类型错误(non-set)
     * @author chen.tengfei
     * @date 2017-03-30
     */
    long long sunionstore(const std::string& dest,
        const std::vector<std::string>& vkeys);

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

public:
    /***************************************************************************
     * zset command
     ***************************************************************************/
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
     * @param [IN] vkeys {const std::vector<std::string>&}
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
        const std::vector<std::string>& vkeys,
        const std::vector<double>* weights = NULL,
        const char* aggregate = "SUM");

    /**
     * @description
     *   计算给定numkeys个有序集合的交集, 并将计算结果放入dest中.
     * @author chen.tengfei
     * @date 2017-04-06
     */
    long long zunionstore(const std::string& dest,
        const std::vector<std::string>& vkeys,
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
    long long zlexcount(const std::string& key,
        const std::string& min, const std::string& max);

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
        std::vector<std::string>& result, int offset = -1, int count = 0);

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

public:
    /***************************************************************************
     * keys command
     ***************************************************************************/

    /**
     * 删除指定的一个或一组keys, 忽略不存在的key
     * @param [IN] vkeys {const std::vector<std::string>&} 指定的一组keys
     * @return {int} 返回实际删除的key的个数, 如下:
     *   0: 未删除任何key
     *  >0: 实际删除的key的个数, 该值可能小于输入key的个数
     *  -1: 出错
     */
    int del(const std::vector<std::string>& vkeys);
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
     * @param [IN] vkeys {const std::vector<std::string>&} 给定的key
     * @return {int} 返回存在的key的个数, 返回值如下:
     *   >0: 存在的key的个数, 如果参数中同一个key被重复多次, 则将会被多次计数
     *    0: key不存在
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int exists(const std::vector<std::string>& vkeys);

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
     * @param [IN] params {MigrateParams*} migrate 选项
     * @return {bool} 迁移是否成功.
     * @author chen.tengfei
     * @date 2017-04-26
     */
    bool migrate(const std::string& host, uint16_t port, const std::string& key,
        int dest_db, long long timeout, MigrateParams* params = NULL);

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
    int move(const std::string& key, int db);

    /**
     * @description
     *   返回指定key关联的Value被引用的次数. 此命令主要用于调试
     * @param [IN] key {const std::string&} 指定的键值
     * @return {int} 返回引用次数, 返回值如下:
     *   >0: 引用次数
     *    0: key不存在
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-05-05
     */
    int object_refcount(const std::string& key);

    /**
     * @description
     *   返回指定key关联的Value的内部表示方式(即压缩方式).
     * @param [IN] key {const std::string&} 指定的键值
     * @param [OUT] encode {std::string&} 存放返回结果(压缩方式), 空值表示key不
     *   存在.
     * @return {bool} 返回操作结果, 返回值如下:
     *    true: 操作成功
     *   false: 操作失败
     * @author chen.tengfei
     * @date 2017-05-05
     */
    bool object_encoding(const std::string& key, std::string& encode);

    /**
     * @description
     *   返回指定key关联的Value的空闲时间(没有读写请求), 单位: 秒
     * @param [IN] key {const std::string&} 指定的键值
     * @return {long long} 返回空闲时间, 返回值如下:
     *   >0: 空闲时间
     *    0: key不存在
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-05-05
     */
    long long object_idletime(const std::string& key);

    /**
     * @description
     *   移除指定key的超时时间.
     * @param [IN] key {const std::string&} 指定的键值
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 成功移除生存时间
     *    0: key不存在或者key没有设置生存时间
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int persist(const std::string& key);

    /**
     * @description
     *   设置key的生存时间. 生存时间超时, key将会被删除
     * @param [IN] key {const std::string&} 给定的key
     * @param [IN] milliseconds {long long} 生存时间, 单位: 毫秒. 如果时间为非正
     *   数, key将被立即删除
     * @return {int} 返回值如下:
     *    1: 设置成功
     *    0: key不存在或生存时间无法被设置
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int pexpire(const std::string& key, long long milliseconds);

    /**
     * @description
     *   用UNIX时间戳设置key的生存时间.
     * @param [IN] key {const std::string&} 给定的key
     * @param [IN] timestamp {time_t} 以毫秒为单位的UNIX时间戳. 如果时间戳表示过
     *   去的时刻, key将被立即删除
     * @return {int} 返回值如下:
     *    1: 设置成功
     *    0: key不存在或时间戳无法被设置
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-04-24
     */
    int pexpireat(const std::string& key,
        unsigned long long milliseconds_timestamp);

    /**
     * @description
     *   返回指定key剩余的生存时间, 单位: 毫秒
     * @param [IN] key {const std::string&} 给定的key
     * @return {long long} 返回剩余生存时间, 返回值如下:
     *   >=0: 剩余生存时间
     *    -1: key存在但是已经过期
     *    -2: key不存在
     * @author chen.tengfei
     * @date 2017-05-09
     */
    long long pttl(const std::string& key);

    /**
     * @description
     *   从当前数据库返回一个随机的key.
     * @param [OUT] out {std::string&} 存储返回的key
     * @return {int} 返回操作结果, 返回值如下:
     *    1: 成功返回一个随机的key
     *    0: 当前数据库为空
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int randomkey(std::string& out);

    /**
     * @description
     *   将key重命名为new_key. 如果key不存在, 则失败; 如果new_key存在, 其值将被
     *   覆盖.
     * @param [IN] key {const std::string&} 给定的key
     * @param [IN] new_key {const std::string&} 新的键名
     * @return {bool} 返回操作结果, 返回值如下:
     *    true: 操作成功
     *   false: 操作失败
     * @author chen.tengfei
     * @date 2017-05-09
     */
    bool rename(const std::string& key, const std::string& new_key);

    /**
     * @description
     *   当new_key不存在时, 将key重命名为new_key. 如果key不存在, 则失败.
     * @param [IN] key {const std::string&} 给定的key
     * @param [IN] new_key {const std::string&} 新的键名
     * @return {int} 返回操作结果, 返回值如下:
     *    1: key 成功命名为 new_key
     *    0: new_key 已经存在
     *   -1: key 不能存在
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int renamenx(const std::string& key, const std::string& new_key);

    /**
     * @description
     *   反序列化给定的序列化值, 并将其存入 key 中. 如果 key 已经存在, 则返回一
     *   个"Target key name is busy"的错误, 除非使用 REPLACE 模式.
     * @param [IN] key {const std::string&} 给定的key.
     * @param [IN] TTL {unsigned long long} key的生存时间, 单位: 毫秒.
     *   如果ttl为0, 则不设置生存时间.
     * @param [IN] serialized_value {const std::string&} 序列化的值.
     * @param [IN] is_replace {bool} 是否使用 REPLACE 模式(redis 3.0或以上版本).
     *    true: 使用 REPLACE 模式.
     *   false: 不使用 REPLACE 模式.
     * @return {bool} 返回操作结果, 返回值如下:
     *    true: 操作成功
     *   false: 操作失败
     * @author chen.tengfei
     * @date 2017-05-09
     */
    bool restore(const std::string& key, unsigned long long TTL,
        const std::string& serialized_value, bool is_replace = false);

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

    /**
     * @description
     *   对key关联的list, set, zset中的元素排序, 并将结果存储在指定的dest中.
     *   默认按照数值类型排序, 比较的元素被看成双精度浮点数类型.
     * @param [IN] key {const std::string&} 给定的key.
     * @param [IN] dest {const std::string&} 存储结果的列表键值.
     * @param [IN] param {SortParams*} 排序参数
     * @return {int} 返回结果列表的长度, 返回值如下:
     *  >=0: 结果列表的长度
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int sort(const std::string& key, const std::string& dest,
        SortParams* params = NULL);

    /**
     * @description
     *   返回key关联的list, set, zset中的排序后的元素.默认按照数值类型排序, 比较
     *   的元素被看成双精度浮点数类型.
     * @param [IN] key {const std::string&} 给定的key.
     * @param [IN] result {std::vector<std::string>&} 存储结果的列表.
     * @param [IN] param {SortParams*} 排序参数
     * @return {int} 返回结果列表的长度, 返回值如下:
     *  >=0: 结果列表的长度
     *   -1: 出错
     * @author chen.tengfei
     * @date 2017-05-09
     */
    int sort(const std::string& key, std::vector<std::string>& result,
        SortParams* params = NULL);

    /**
     * @description
     *   改变给定key的最近访问时间, 忽略不存在的key.
     * @param [IN] vkeys {const std::vector<std::string>&} 指定的一组keys
     * @return {int} 返回被touched的key的个数, 如下:
     *   0: 没有key收到影响
     *  >0: 实际受到影响的key的个数, 该值可能小于输入key的个数
     *  -1: 出错
     * @author chen.tengfei
     * @date 2017-05-10
     */
    int touch(const std::vector<std::string>& vkeys);
    int touch(const std::string& key);

    /**
     * @description
     *   返回指定key的剩余生存时间, 单位: 秒
     * @param [IN] key {const std::string&} 给定的key
     * @return {long long} 返回剩余生存时间, 返回值如下:
     *   >=0: 剩余生存时间
     *    -1: key存在但是已经过期
     *    -2: key不存在
     * @author chen.tengfei
     * @date 2017-05-10
     */
    long long ttl(const std::string& key);

    /**
     * @description
     *   返回key所存储的value的数据结构类型, 可以返回string, list, set, zset 和
     *   hash等不同的类型.
     * @param [IN] key {const std::string&} 给定的key
     * @return {std::string} 返回存储的数据结构类型, 为空表示key不存在.
     * @author chen.tengfei
     * @date 2017-05-10
     */
    std::string type(const std::string& key);

public:
    /***************************************************************************
     * server command
     ***************************************************************************/   
    /**
     * @description
     *   该命令用于让 redis 异步执行一个 AOF(AppendOnly File) 文件重写操作. 重写
     *   操作会创建一个当前AOF文件的小而优化的版本. 即使 BGREWRITEAOF 执行失败,
     *   也不会有任何数据丢失, 因为旧的AOF文件在 BGREWRITEAOF 成功之前不会被修改.
     *   AOF 重写由 Redis 自行触发, BGREWRITEAOF 仅仅用于手动触发重写操作.
     * @return {bool} 始终返回 true
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool bgrewriteaof();

    /**
     * @description
     *   后台保存DB. 会立即返回 OK 状态码. Redis forks, 父进程继续提供服务以供客
     *   户端调用, 子进程将DB数据保存到磁盘然后退出. 如果操作成功, 可以通过客户
     *   端命令 LASTSAVE 来检查操作结果.
     * @return {std::string} 
     * @author chen.tengfei
     * @date 2017-06-07
     */
    std::string bgsave();

    /**
     * @description
     *   返回当前连接由 CLIENT SETNAME 设置的名字. 如果没有用 CLIENT SETNAME 设
     *   置名字, 将返回一个空的回复.
     * @param [OUT] name {std::string&} 当前连接的名字.
     * @return {bool} 返回操作结果.
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool clientGetname(std::string& name);

    /**
     * @description
     *   关闭一个指定的连接. 关闭方式如下:
     *     CLIENT KILL ip:port
     *   ip:port是CLIENT LIST 命令里面列出的客户端连接之一, 匹配 addr 域
     * @param [IN] addr {const std::string&} 客户端地址和接口
     * @return {bool} 返回操作结果, 返回值如下:
     *    true: 客户端存在并且已被关闭
     *   false: 客户端不存在或出错
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool clientKill(const std::string& addr);

    /**
     * @description
     *   关闭符合指定 filter 的客户端.
     * @param [IN] params {KillFilterParams} 客户端的不同属性参数.
     * @return {int} 返回被关闭的客户端的数量.
     * @author chen.tengfei
     * @date 2017-06-07
     */
    int clientKill(KillFilterParams params);

    /**
     * @description
     *   返回所有连接到服务器的客户端信息和统计数据.     
     * @param [OUT] list {std::vector<std::map<std::string, std::string>*>*}
     *   解析后的客户端信息和统计数据列表, 列表中的每个元素由调用者负责释放.
     * @return {std::string} 返回信息的格式如下:
     *   1. 每行对应一个已连接的客户端(以LF分隔)
     *   2. 每行字符串由一系列 属性=值(property=value)形式的域组成, 每个域之间以
     *      空格分开.
     *   返回信息各个字段的含义请参考: https://redis.io/commands/client-list
     * @author chen.tengfei
     * @date 2017-06-11
     */
    std::string clientList(
        std::vector<std::map<std::string, std::string>*>* list = NULL);

    /**
     * @description
     *   暂停所有的客户端, 直到超时. 该命令执行以下操作:
     *     1. 停止 normal 和 pub/sub 客户端上所有正在等待执行的挂起命令, 但是交
     *        互式的 slave 客户端不会被暂停.
     *     2. 调用该命令的客户端不会暂停, 并立即返回 OK.
     *     3. 当超时时间已过, 所有的客户端将会被解除阻塞: 这将会触发在暂停期间累
     *        积在每一个客户端的查询缓冲区(query buffer)中的所有命令的执行过程.
     * @param [IN] timeout {long long} 指定的超时时间.
     * @return {bool} 返回true.
     * @author chen.tengfei
     * @date 2017-06-13
     */
    bool clientPause(long long timeout);

    /**
     * @description
     *   设置 redis服务器回复客户端的模式. 模式如下:
     *   ON: 默认模式, 服务器为每一个命令返回一个回复;
     *   OFF: 该模式下, 服务器不会回复任何客户端命令;
     *   SKIP: 跳过紧接着的下一条命令的回复.
     * @param [IN] reply {CLIENT_REPLY} 
     * @return {int} 返回执行状态:
     *    1: 参数为 ON
     *    0: 参数为 OFF或SKIP
     * @author chen.tengfei
     * @date 2017-06-13
     */
    int clientReply(CLIENT_REPLY reply);

    /**
     * @description
     *   为当前的连接设置名字, 设置的名字可由 CLIENT LIST 命令查看.
     * @param [IN] connName {const std::string&} 
     * @return {bool} 设置成功返回true.
     * @author chen.tengfei
     * @date 2017-06-13
     */
    bool clientSetname(const std::string& connName);

    /**
     * @description
     *   删除所有数据库中的所有数据, 该命令永远不会失败.
     * @return {bool}
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool flushAll();

    /**
     * @description
     *   删除当前数据库中的所有数据, 该命令永远不会失败.
     * @return {bool}
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool flushDB();

    /**
     * @description
     *   返回最近一次数据集保存操作执行成功时的时间戳. 客户端可以通过如下方式判
     *   断 BGSAVE 命令是否执行成功:
     *    1. 读取 LASTSAVE 的返回值
     *    2. 执行 BGSAVE 命令
     *    3. 每隔几秒查看 LASTSAVE 的返回值是否发生变化 
     * @return {long long} 最近一次数据集保存操作执行成功时的时间戳.
     * @author chen.tengfei
     * @date 2017-06-07
     */
    long long lastsave();

    /**
     * @description
     *   以同步方式保存数据集, 产生一个 redis 实例中所有数据的时间点快照, 并保存
     *   在 RDB 文件中
     * @return {bool} 始终返回 true
     * @author chen.tengfei
     * @date 2017-06-07
     */
    bool save();

public:
    /***************************************************************************
     * connection command
     ***************************************************************************/
    /**
     * @description
     *   在有加密保护的 redis-server 上请求身份认证. redis 可以设置在客户端执行
     *   commands请求前需要通过密码验证. 通过修改配置文件的 requirepass 指令就可
     *   以设置密码.
     * @param [IN] password {const std::string} 在 redis 配置文件中指定的认证密码
     * @return {bool}
     *    true: 密码匹配, 服务端可以接收后续命令
     *   false: 匹配失败, 客户端需要重试新的密码
     * @author chen.tengfei
     * @date 2017-06-06
     */
    bool auth(const std::string password);

    /**
     * @description
     *   服务端将会返回 message.
     * @param [IN] message {const std::string} 
     * @return {std::string} 服务端返回的消息
     * @author chen.tengfei
     * @date 2017-06-06
     */
    std::string echo(const std::string message);

    /**
     * @description
     *   该命令经常用来测试一个连接是否还是可用的, 或者用来测试一个连接的延时.
     * @param [IN] message {const std::string*} 发送给服务端的信息, 为空时, 服务
     *   端返回 PONG
     * @return {std::string} 服务端返回的消息
     * @author chen.tengfei
     * @date 2017-06-06
     */
    std::string ping(const std::string* message);
    bool ping();

    /**
     * @description
     *   请求服务器关闭连接.
     * @return {bool} 始终返回 true.
     * @author chen.tengfei
     * @date 2017-06-06
     */
    bool quit();

    /**
     * @description
     *   选择一个数据库. 数据库下标从0开始. 非集群模式的server实例有16个数据库,
     *   即1~15. 集群模式的server实例只有一个数据库. 该命令不允许在集群模式下使
     *   用.
     * @param [IN] index {int} 数据库下标, 基于0
     * @return {bool} 返回操作结果, true表示数据库切换成功
     * @author chen.tengfei
     * @date 2017-06-04
     */
    bool select(int index);

private:
    RedisClient* m_client;
};

#endif /* __REDIS_H__ */

