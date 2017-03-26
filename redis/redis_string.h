#ifndef __REDIS_STRING_H__
#define __REDIS_STRING_H__

#include <string>
#include <vector>
#include <map>

#include "redis_command.h"

class redis_client;

class redis_string : public redis_command
{
public:
    enum BITOP {
        AND,
        OR,
        NOT,
        XOR,

        SIZE_BITOP
    };
    static const char* BITOP_STR[SIZE_BITOP];

public:
    redis_string(redis_client* client):redis_command(client){}
    ~redis_string(){}

public:
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
    long long bitconut(const std::string& key);
    long long bitconut(const std::string& key, int start, int end);

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
     *   或key值不存在返回空.
     * @param [IN] keys {const std::vector<std::string>&}
     *   一组字符串对象的key
     * @param [OUT] result {std::vector<std::string*>&}
     *   keys所关联的value, 需要调用者手动释放内存
     * @return {bool} 返回true表示执行成功, 该调用不会失败
     */
    bool mget(const std::vector<std::string>& keys,
              std::vector<std::string*>& result);
    bool mget(const std::vector<std::string>& keys,
              std::vector<std::string*>* result);

    /**
     * @description
     *   把key关联到对应的value上, 会覆盖原有的value值, 行为和set类似.
     * @param [IN] keyValues {const std::map<std::string, std::string>&} 键值对
     * @return {bool} 返回true表示执行成功, 该调用不会失败
     */
    bool mset(const std::map<std::string, std::string>& keyValues);

    /**
     * @description
     *   把key关联到对应的value上, 只要有一个key存在, 整个msetnx都不会执行.
     * @param [IN] keyValues {const std::map<std::string, std::string>&} 键值对
     * @return {bool} 返回true表示执行成功, 该调用不会失败
     */
    bool msetnx(const std::map<std::string, std::string>& keyValues);

    /**
     * @description
     *   对存储在指定key的数值执行加1操作. 如果key不存在, 自增前将key的值设为0
     * @param [IN] key {const std::string&} 指定自增的key
     * @param [OUT] result {long long*} 非空时存储自增结果
     * @return {bool} 表示自增操作是否成功, 如下:
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
     * @return {bool} 表示自增操作是否成功, 如下:
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
     * @return {bool} 表示自增操作是否成功, 如下:
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
     * @return {bool} 表示自减操作是否成功, 如下:
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
     * @return {bool} 表示自减操作是否成功, 如下:
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
     * @return {bool} 表示自减操作是否成功, 如下:
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
     * @return {bool} 操作是否成功, 如下:
     *    true: 操作成功
     *   false: key的value类型错误(non-string)
     */
    bool strlen(const std::string& key, long long& length);

private:
    bool set_string(const std::string& key, const std::string& value,
                    const char* ex_or_px, long long second,
                    const char* nx_or_xx);
    bool incoper(const char* cmd, const std::string& key,
                 long long* inc, long long* result);
};

#endif /* __REDIS_STRING_H__ */
