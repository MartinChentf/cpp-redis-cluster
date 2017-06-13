#ifndef __REDIS_SERVER_H__
#define __REDIS_SERVER_H__

#include <string>
#include <vector>
#include <map>

#include "ProtocolCommand.h"
#include "redis_command.h"

class KillFilterParams;

class redis_server : virtual public redis_command
{
public:
    redis_server(const std::string& host, uint16_t port);
    ~redis_server(){}

public:
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

};

/**
 * 保存 CLIENT KILL 命令的 filter 参数
 */
class KillFilterParams
{
public:
    std::vector<std::string>& getParams() { return params; }

    /**
     * @description
     *   关闭 addr 参数指定的一个连接.
     *   addr 参数匹配 CLIENT LIST 命令返回值的addr 域.
     * @param [IN] addr {const std::string&} 客户端地址和接口, 形式如 ip:port.
     * @return {KillFilterParams&} 返回 KillFilterParams 类对象.
     * @author chen.tengfei
     * @date 2017-06-08
     */
    KillFilterParams& ADDR(const std::string& addr);

    /**
     * @description
     *   通过客户端的唯一ID域关闭一个连接.
     *   id 参数匹配 CLIENT LIST 命令返回值的id 域.
     * @param [IN] id {int} 客户端的唯一ID.
     * @return {KillFilterParams&} 返回 KillFilterParams 类对象.
     * @author chen.tengfei
     * @date 2017-06-08
     */
    KillFilterParams& ID(int id);

    /**
     * @description
     *   关闭指定客户端类型的所有连接. 客户端类型有如下几种:
     *    normal : TYPE_NORMAL
     *    master : TYPE_MASTER, 从 v3.2 版本开始引入
     *    slave  : TYPE_SLAVE
     *    pubsub : TYPE_PUBSUB
     *   提示: 由 MONITOR 命令阻塞的客户端被认为属于 normal 类型
     * @param [IN] type {CLIENT_TYPE} 客户端类型
     * @return {KillFilterParams&} 返回 KillFilterParams 类对象.
     * @author chen.tengfei
     * @date 2017-06-08
     */
    KillFilterParams& TYPE(CLIENT_TYPE type);

    /**
     * @description
     *   该选项默认值是yes, 表示调用该命令的客户端不会被关闭, 如果该选项设置成no,
     *   表示调用该命令的客户端也会被关闭.
     * @param [IN] yes {bool} 
     * @return {KillFilterParams&} 返回 KillFilterParams 类对象.
     * @author chen.tengfei
     * @date 2017-06-08
     */
    KillFilterParams& SKIPMME(bool yes = true);

private:
    std::vector<std::string> params;
};

#endif /* __REDIS_SERVER_H__ */
