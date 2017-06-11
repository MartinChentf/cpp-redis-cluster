#include <pthread.h>

#include "gtest/gtest.h"
#include "gt_redis_list.h"
#include "gt_common.h"

#include "Util.h"
#include "redis.h"

redis* redis_list_test::m_pRedis = NULL;

void redis_list_test::SetUpTestCase() {
    m_pRedis = new redis(gt_component::Instance().get_host(0),
                         gt_component::Instance().get_port(0));

    m_pRedis->del("foo");
}

void redis_list_test::TearDownTestCase() {
    delete m_pRedis;
    m_pRedis = NULL;
}

void redis_list_test::SetUp()
{
    std::vector<std::string> values;
    values.push_back("one");
    values.push_back("two");
    values.push_back("three");

    redis_list_test::m_pRedis->lpush("foo", values);
}

void redis_list_test::TearDown()
{
    redis_list_test::m_pRedis->del("foo");
}

TEST_F(redis_list_test, lrange)
{
    std::vector<std::string> result;

    // 返回全部元素
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, two, one", Util::join(result, ", "));
    result.clear();

    // 返回部分元素
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 1, 2, result));
    EXPECT_EQ("two, one", Util::join(result, ", "));
    result.clear();

    // start > stop
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 2, 0, result));
    EXPECT_EQ("", Util::join(result, ", "));
    result.clear();

    // stop大于实际列表末尾元素下标
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 1, 10, result));
    EXPECT_EQ("two, one", Util::join(result, ", "));
    result.clear();

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_list_test::m_pRedis->lrange("foo", 1, 2, result));
}

TEST_F(redis_list_test, lpush)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");

    // 在列表头部插入新元素
    EXPECT_EQ(5, redis_list_test::m_pRedis->lpush("foo", values));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("five, four, three, two, one", Util::join(result, ", "));
    
    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lpush("foo", values));
}

TEST_F(redis_list_test, lpushx)
{
    std::vector<std::string> result;

    // key存在的情况
    EXPECT_EQ(4, redis_list_test::m_pRedis->lpushx("foo", "four"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("four, three, two, one", Util::join(result, ", "));

    // key不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->lpushx("foo", "four"));

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lpushx("foo", "four"));
}

TEST_F(redis_list_test, lindex)
{
    std::string result;

    // 1. 获取index处的元素
    EXPECT_EQ(1, redis_list_test::m_pRedis->lindex("foo", 1, result));
    EXPECT_EQ("two", result);

    // 2. index为负值的情况
    EXPECT_EQ(1, redis_list_test::m_pRedis->lindex("foo", -1, result));
    EXPECT_EQ("one", result);

    // 3. index超出范围的情况
    EXPECT_EQ(0, redis_list_test::m_pRedis->lindex("foo", 3, result));

    // 4. key不存在的情况(相当于index超出范围)
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->lindex("foo", 0, result));
    
    // 5. key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lindex("foo", 0, result));
}

TEST_F(redis_list_test, linsert)
{
    std::vector<std::string> result;

    // 插入指定元素之前
    EXPECT_EQ(4, redis_list_test::m_pRedis->linsert("foo", true, "two", "four"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, four, two, one", Util::join(result, ", "));
    result.clear();

    // 插入指定元素之后
    EXPECT_EQ(5, redis_list_test::m_pRedis->linsert("foo", false, "two", "five"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, four, two, five, one", Util::join(result, ", "));
    result.clear();

    // 指定元素不存在的情况
    EXPECT_EQ(-1, redis_list_test::m_pRedis->linsert("foo", true, "six", "five"));

    // key不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->linsert("foo", true, "two", "five"));

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->linsert("foo", true, "two", "four"));
}

TEST_F(redis_list_test, llen)
{
    // 返回列表长度
    EXPECT_EQ(3, redis_list_test::m_pRedis->llen("foo"));

    // key不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->llen("foo"));
    
    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->llen("foo"));
}

TEST_F(redis_list_test, lpop)
{
    std::string result;

    // 弹出列表中第一个元素
    EXPECT_EQ(1, redis_list_test::m_pRedis->lpop("foo", result));
    EXPECT_EQ("three", result);

    // key不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->lpop("foo", result));
    
    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lpop("foo", result));
}

TEST_F(redis_list_test, lrem_from_head)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("two");
    values.push_back("one");
    values.push_back("two");
    redis_list_test::m_pRedis->lpush("foo", values);

    // 从头部开始移除
    EXPECT_EQ(2, redis_list_test::m_pRedis->lrem("foo", 2, "two"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("one, three, two, one", Util::join(result, ", "));

    // vaule不存在的情况
    EXPECT_EQ(0, redis_list_test::m_pRedis->lrem("foo", 2, "four"));

    // key不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->lrem("foo", 2, "two"));

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->lrem("foo", 2, "two"));
}

TEST_F(redis_list_test, lrem_from_tail)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("two");
    values.push_back("one");
    values.push_back("two");
    redis_list_test::m_pRedis->lpush("foo", values);

    // 从尾部开始移除
    EXPECT_EQ(2, redis_list_test::m_pRedis->lrem("foo", -2, "two"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("two, one, three, one", Util::join(result, ", "));
}

TEST_F(redis_list_test, lrem_all)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("two");
    values.push_back("one");
    values.push_back("two");
    redis_list_test::m_pRedis->lpush("foo", values);

    // 移除所有
    EXPECT_EQ(3, redis_list_test::m_pRedis->lrem("foo", 0, "two"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("one, three, one", Util::join(result, ", "));
}

TEST_F(redis_list_test, lset)
{
    std::vector<std::string> result;

    // 设置新值
    EXPECT_EQ(true, redis_list_test::m_pRedis->lset("foo", 1, "four"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, four, one", Util::join(result, ", "));

    // index越界的情况
    EXPECT_EQ(false, redis_list_test::m_pRedis->lset("foo", 3, "four"));
    
    // key不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(false, redis_list_test::m_pRedis->lset("foo", 1, "four"));

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_list_test::m_pRedis->lset("foo", 1, "four"));
}

TEST_F(redis_list_test, ltrim)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");
    values.push_back("six");
    values.push_back("seven");
    redis_list_test::m_pRedis->lpush("foo", values);

    EXPECT_EQ(true, redis_list_test::m_pRedis->ltrim("foo", 2, 5));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("five, four, three, two", Util::join(result, ", "));

    // key不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(true, redis_list_test::m_pRedis->ltrim("foo", 2, 5));

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_list_test::m_pRedis->ltrim("foo", 2, 5));
}

TEST_F(redis_list_test, ltrim_start_gt_stop)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");
    values.push_back("six");
    values.push_back("seven");
    redis_list_test::m_pRedis->lpush("foo", values);

    // start > stop
    EXPECT_EQ(true, redis_list_test::m_pRedis->ltrim("foo", 6, 3));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("", Util::join(result, ", "));
}

TEST_F(redis_list_test, ltrim_stop_gt_end)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");
    values.push_back("six");
    values.push_back("seven");
    redis_list_test::m_pRedis->lpush("foo", values);

    // stop大于列表下标
    EXPECT_EQ(true, redis_list_test::m_pRedis->ltrim("foo", 2, 10));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("five, four, three, two, one", Util::join(result, ", "));
}

TEST_F(redis_list_test, rpop)
{
    std::string result;

    // 弹出列表中第一个元素
    EXPECT_EQ(1, redis_list_test::m_pRedis->rpop("foo", result));
    EXPECT_EQ("one", result);

    // key不存在
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->rpop("foo", result));
    
    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpop("foo", result));
}

TEST_F(redis_list_test, rpoplpush)
{
    std::string result;
    std::vector<std::string> vaules;

    // 从src末尾弹出一个元素并移入dest头部
    EXPECT_EQ(1, redis_list_test::m_pRedis->rpoplpush("foo", "{foo}1", result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("three, two", Util::join(vaules, ", "));
    vaules.clear();
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("three, two", Util::join(vaules, ", "));
    vaules.clear();

    // src不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->rpoplpush("foo", "{foo}1", result));

    // src类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpoplpush("foo", "{foo}1", result));
}

TEST_F(redis_list_test, rpoplpush_src_same_with_dest)
{
    std::string result;
    std::vector<std::string> vaules;

    // 从src末尾弹出一个元素并移入dest头部
    EXPECT_EQ(1, redis_list_test::m_pRedis->rpoplpush("foo", "foo", result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("one, three, two", Util::join(vaules, ", "));

    // dest类型错误的情况
    redis_list_test::m_pRedis->set("{foo}1", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpoplpush("foo", "{foo}1", result));

    redis_list_test::m_pRedis->del("{foo}1");
}

TEST_F(redis_list_test, rpush)
{
    std::vector<std::string> result;
    std::vector<std::string> values;
    values.push_back("four");
    values.push_back("five");

    // 在列表末尾插入新元素
    EXPECT_EQ(5, redis_list_test::m_pRedis->rpush("foo", values));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, two, one, four, five", Util::join(result, ", "));

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpush("foo", values));
}

TEST_F(redis_list_test, rpushx)
{
    std::vector<std::string> result;

    // key存在的情况
    EXPECT_EQ(4, redis_list_test::m_pRedis->rpushx("foo", "four"));
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, result));
    EXPECT_EQ("three, two, one, four", Util::join(result, ", "));

    // key不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->rpushx("foo", "four"));

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->rpushx("foo", "four"));
}

TEST_F(redis_list_test, blpop_non_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");
    std::string key, value;

    // 弹出列表中第一个元素
    EXPECT_EQ(1, redis_list_test::m_pRedis->blpop(keys, 2, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("three", value);

    // key为空列表, 超时的情况
    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    EXPECT_EQ(0, redis_list_test::m_pRedis->blpop(keys, 2, key, value));

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->blpop(keys, 2, key, value));

    // 其他错误(超时时间为负值) 
    EXPECT_EQ(-1, redis_list_test::m_pRedis->blpop(keys, -2, key, value));
}

TEST_F(redis_list_test, brpop_non_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");
    std::string key, value;

    // 弹出列表末尾元素
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpop(keys, 2, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("one", value);

    // key为空列表, 超时的情况
    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    EXPECT_EQ(0, redis_list_test::m_pRedis->brpop(keys, 2, key, value));

    // key类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpop(keys, 2, key, value));

    // 其他错误(超时时间为负值) 
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpop(keys, -2, key, value));
}

TEST_F(redis_list_test, brpoplpush_non_block)
{
    std::string result;
    std::vector<std::string> vaules;

    // 从src末尾弹出一个元素并移入dest头部
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", 2, result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("three, two", Util::join(vaules, ", "));
    vaules.clear();
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("{foo}:1", 0, -1, vaules));
    EXPECT_EQ("one", Util::join(vaules, ", "));
    vaules.clear();

    // src不存在的情况
    redis_list_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", 2, result));

    // src类型错误的情况
    redis_list_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", 2, result));
    
    // 其他错误(超时时间为负值)
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", -2, result));

    redis_list_test::m_pRedis->del("{foo}:1");
}

TEST_F(redis_list_test, brpoplpush_src_same_with_dest)
{
    std::string result;
    std::vector<std::string> vaules;

    // 从src末尾弹出一个元素并移入dest头部
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpoplpush("foo", "foo", 2, result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("one, three, two", Util::join(vaules, ", "));

    // dest类型错误的情况
    redis_list_test::m_pRedis->set("{foo}:1", "hello");
    EXPECT_EQ(-1, redis_list_test::m_pRedis->brpoplpush("foo", "{foo}:1", 2, result));

    redis_list_test::m_pRedis->del("{foo}:1");
}

// 模拟另外一个客户端插入数据
void* other_client(void*)
{
    sleep(5);
    
    redis* pRedis = new redis(gt_component::Instance().get_host(0),
                              gt_component::Instance().get_port(0));

    std::vector<std::string> values;
    values.push_back("one");
    pRedis->lpush("foo", values);

    delete pRedis;
    return 0;
}

TEST_F(redis_list_test, blpop_with_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");

    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    // 启动一个线程模拟另外一个redis client
    pthread_t id;
    EXPECT_EQ(0, pthread_create(&id, NULL, other_client, NULL));

    // 弹出列表中第一个元素
    std::string key, value;
    EXPECT_EQ(1, redis_list_test::m_pRedis->blpop(keys, 0, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("one", value);

    pthread_join(id, NULL);
}

TEST_F(redis_list_test, brpop_with_block)
{
    std::vector<std::string> keys;
    keys.push_back("foo");

    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    // 启动一个线程模拟另外一个redis client
    pthread_t id;
    EXPECT_EQ(0, pthread_create(&id, NULL, other_client, NULL));

    // 弹出列表末尾元素
    std::string key, value;
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpop(keys, 0, key, value));
    EXPECT_EQ("foo", key);
    EXPECT_EQ("one", value);

    pthread_join(id, NULL);
}

TEST_F(redis_list_test, brpoplpush_with_block)
{
    std::string result;
    std::vector<std::string> vaules;

    redis_list_test::m_pRedis->ltrim("foo", 1, 0);
    // 启动一个线程模拟另外一个redis client
    pthread_t id;
    EXPECT_EQ(0, pthread_create(&id, NULL, other_client, NULL));

    // 从src末尾弹出一个元素并移入dest头部
    EXPECT_EQ(1, redis_list_test::m_pRedis->brpoplpush("foo", "foo", 0, result));
    EXPECT_EQ("one", result);
    EXPECT_EQ(true, redis_list_test::m_pRedis->lrange("foo", 0, -1, vaules));
    EXPECT_EQ("one", Util::join(vaules, ", "));

    pthread_join(id, NULL);
}

