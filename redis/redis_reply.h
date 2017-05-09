#ifndef __REDIS_REPLY_H__
#define __REDIS_REPLY_H__

#include <vector>
#include <string>

typedef enum {
    REDIS_REPLY_UNKOWN,     // 0
    REDIS_REPLY_STRING,     // 1
    REDIS_REPLY_ARRAY,      // 2
    REDIS_REPLY_INTEGER,    // 3
    REDIS_REPLY_NIL,        // 4
    REDIS_REPLY_STATUS,     // 5
    REDIS_REPLY_ERROR,      // 6
} t_redis_reply;

static const char * const REPLY_TYPE[] =
{
    "REDIS_REPLY_UNKOWN",   // 0
    "REDIS_REPLY_STRING",   // 1
    "REDIS_REPLY_ARRAY",    // 2
    "REDIS_REPLY_INTEGER",  // 3
    "REDIS_REPLY_NIL",      // 4
    "REDIS_REPLY_STATUS",   // 5
    "REDIS_REPLY_ERROR"     // 6
};

/**
 * @description
 *   存储经过解析后的 redis-server 应答结果的类. 调用者把解析后的 redis-server
 *   应答数据存入 redis_reply 类对象.
 * @author chen.tengfei
 * @date 2017-04-22
 */
class redis_reply
{
public:
    redis_reply();
    ~redis_reply();

public:
    /**
     * @description
     *   返回当前 redis_reply 节点的数据类型
     * @return {t_redis_reply} 
     * @author chen.tengfei
     * @date 2017-04-23
     */
    t_redis_reply get_type() const { return m_type; }

    /**
     * @description
     *   返回当前节点存储的 redis_reply 对象的个数
     * @return {size_t} 返回值与节点数据类型的对应关系如下:
     *   REDIS_REPLY_UNKOWN: -1
     *   REDIS_REPLY_NIL: 0
     *   REDIS_REPLY_ERROR: 1
     *   REDIS_REPLY_STATUS: 1
     *   REDIS_REPLY_STRING: 1
     *   REDIS_REPLY_INTEGER: 1
     *   REDIS_REPLY_ARRAY: 子节点的个数
     * @author chen.tengfei
     * @date 2017-04-23
     */
    size_t get_size() const;

    /**
     * @description
     *   当数据类型是 REDIS_REPLY_ARRAY 类型时, 返回指定下标的子节点对象.
     * @param [IN] idx {size_t} 下标值
     * @return {const redis_reply*} 如果下标越界或者不存在子节点, 返回空置
     * @author chen.tengfei
     * @date 2017-04-23
     */
    const redis_reply* get_element(size_t idx) const;

    /**
     * @description
     *   当数据类型是 REDIS_REPLY_STATUS 类型时, 返回当前节点的状态信息
     * @return {std::string} 当前节点的状态信息, 返回空字符串表示数据类型不匹配
     * @author chen.tengfei
     * @date 2017-04-23
     */
    std::string get_status() const;

    /**
     * @description
     *   当数据类型是 REDIS_REPLY_ERROR 类型时, 返回当前节点的错误信息
     * @return {std::string} 当前节点的错误信息, 返回空字符串表示数据类型不匹配
     * @author chen.tengfei
     * @date 2017-04-23
     */
    std::string get_error() const;

    /**
     * @description
     *   当数据类型是 REDIS_REPLY_STRING 类型时, 返回当前节点的字符串信息
     * @return {std::string} 当前节点存储的字符串
     * @author chen.tengfei
     * @date 2017-04-23
     */
    std::string get_string() const { return m_str; }

    /**
     * @description
     *   当数据类型是 REDIS_REPLY_INTEGER 类型时, 返回当前节点存储的长整型数据
     * @return {long long} 当前节点存储的长整型数据
     * @author chen.tengfei
     * @date 2017-04-23
     */
    long long get_integer() const { return m_integer; }

    /**
     * @description
     *   当数据类型是 REDIS_REPLY_STRING 类型时, 返回字符串对应的双精度浮点数
     * @param [OUT] success {bool*} 当指针非空时, 存储操作结果是否成功
     * @return {double} 双精度浮点数值
     * @author chen.tengfei
     * @date 2017-04-23
     */
    double get_double(bool* success = NULL) const;

public:
    /**
     * @description
     *   设置当前redis_reply节点的数据类型
     * @param [IN] type {t_redis_reply} 节点的数据类型
     * @return {redis_reply&} 当前节点的this指针
     * @author chen.tengfei
     * @date 2017-04-23
     */
    redis_reply& set_type(t_redis_reply type);

    /**
     * @description
     *   向当前节点存放数据. 当前节点为非 REDIS_REPLY_ARRAY 类型时使用.
     *   注意: 调用本函数前必须先调用 set_type 函数指定正确的数据类型, 否则可能
     *         会造成存储数据混乱
     * @param [IN] buff {const std::string&} 
     * @return {redis_reply&} 当前节点的this指针
     * @author chen.tengfei
     * @date 2017-04-23
     */
    redis_reply& put(const std::string& buff);
    redis_reply& put(const char* buff, int len);

    /**
     * @description
     *   向当前节点存放数据. 当前节点为 REDIS_REPLY_ARRAY 类型时使用.
     *   注意: 调用本函数前必须先调用 set_type 函数指定正确的数据类型, 否则可能
     *         会造成存储数据混乱
     * @param [IN] rr {const redis_reply*} 
     * @return {redis_reply&} 当前节点的this指针
     * @author chen.tengfei
     * @date 2017-04-23
     */
    redis_reply& put(const redis_reply* rr);

private:
    void clear();

private:
    // REDIS_REPLY_*
    t_redis_reply m_type;
    // The integer when type is REDIS_REPLY_INTEGER
    long long m_integer;
    // Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING
    std::string m_str;
    // elements vector for REDIS_REPLY_ARRAY
    std::vector<const redis_reply*> m_element;
};

#endif /* __REDIS_REPLY_H__ */
