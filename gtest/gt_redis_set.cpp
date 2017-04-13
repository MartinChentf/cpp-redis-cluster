#include "gtest/gtest.h"
#include "gt_redis_set.h"
#include "gt_common.h"

#include "redis_client.h"
#include "redis_set.h"
#include "redis_key.h"
#include "redis_helper.h"
#include "redis_string.h"

redis_client* redis_set_test::m_pClient = NULL;
redis_string* redis_set_test::m_pStr = NULL;
redis_set* redis_set_test::m_pSet = NULL;
redis_key* redis_set_test::m_pKey = NULL;

void redis_set_test::SetUpTestCase() {
    m_pClient = new redis_client(gt_component::Instance().get_host(),
                                 gt_component::Instance().get_port());
    m_pSet = new redis_set(m_pClient);
    m_pKey = new redis_key(m_pClient);
    m_pStr = new redis_string(m_pClient);
}

void redis_set_test::TearDownTestCase() {
    delete m_pClient;
    m_pClient = NULL;
    delete m_pSet;
    m_pSet = NULL;
    delete m_pKey;
    m_pKey = NULL;
    delete m_pStr;
    m_pStr = NULL;
}

void redis_set_test::SetUp()
{
    std::vector<std::string> member;
    member.push_back("one");
    member.push_back("two");
    member.push_back("three");

    m_pSet->sadd("foo", member);
}

void redis_set_test::TearDown()
{
    m_pKey->del("foo");
}

TEST_F(redis_set_test, smember)
{
    std::vector<std::string> result;

    // 获取集合成员
    EXPECT_EQ(3, redis_set_test::m_pSet->smembers("foo", result));
    EXPECT_EQ("two, one, three", redis_helper::join(result, ", "));

    // key不存在
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->smembers("foo", result));

    // key的类型错误
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->smembers("foo", result));
}

TEST_F(redis_set_test, sismember)
{
    // 判断指定成员是否存在
    EXPECT_EQ(1, redis_set_test::m_pSet->sismember("foo", "one"));
    EXPECT_EQ(1, redis_set_test::m_pSet->sismember("foo", "two"));
    EXPECT_EQ(0, redis_set_test::m_pSet->sismember("foo", "four"));

    // key不存在
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->sismember("foo", "one"));

    // key的类型错误
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->sismember("foo", "one"));
}

TEST_F(redis_set_test, sadd)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");

    // 添加新成员
    EXPECT_EQ(1, redis_set_test::m_pSet->sadd("foo", member));
    EXPECT_EQ(1, redis_set_test::m_pSet->sismember("foo", "four"));
    EXPECT_EQ(1, redis_set_test::m_pSet->sismember("foo", "two"));

    // key的类型错误
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->sadd("foo", member));
}

TEST_F(redis_set_test, scard)
{
    // 返回集合成员基数
    EXPECT_EQ(3, redis_set_test::m_pSet->scard("foo"));

    // key不存在
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pSet->scard("foo"));

    // key的类型错误
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pSet->scard("foo"));
}

TEST_F(redis_set_test, sdiff)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pSet->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // 返回集合之间差集
    EXPECT_EQ(true, redis_set_test::m_pSet->sdiff(keys, result));
    EXPECT_EQ("one, three", redis_helper::join(result, ", "));
    result.clear();

    // 后面的key不存在
    redis_set_test::m_pKey->del("{foo}:1");
    EXPECT_EQ(true, redis_set_test::m_pSet->sdiff(keys, result));
    EXPECT_EQ("two, one, three", redis_helper::join(result, ", "));
    result.clear();

    // 第一个key不存在
    redis_set_test::m_pKey->del("foo");
    EXPECT_EQ(true, redis_set_test::m_pSet->sdiff(keys, result));
    EXPECT_EQ("", redis_helper::join(result, ", "));
    result.clear();

    // key的类型错误
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_set_test::m_pSet->sdiff(keys, result));
}

TEST_F(redis_set_test, sinter)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pSet->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // 返回集合之间交集
    EXPECT_EQ(true, redis_set_test::m_pSet->sinter(keys, result));
    EXPECT_EQ("two", redis_helper::join(result, ", "));
    result.clear();

    // 有的key为空
    redis_set_test::m_pKey->del("{foo}:1");
    EXPECT_EQ(true, redis_set_test::m_pSet->sinter(keys, result));
    EXPECT_EQ("", redis_helper::join(result, ", "));
    result.clear();

    // key的类型错误
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_set_test::m_pSet->sinter(keys, result));
}

TEST_F(redis_set_test, sunion)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pSet->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // 返回集合之间并集
    EXPECT_EQ(true, redis_set_test::m_pSet->sunion(keys, result));
    EXPECT_EQ("four, two, one, three", redis_helper::join(result, ", "));
    result.clear();

    // 有的key为空
    redis_set_test::m_pKey->del("{foo}:1");
    EXPECT_EQ(true, redis_set_test::m_pSet->sunion(keys, result));
    EXPECT_EQ("two, one, three", redis_helper::join(result, ", "));
    result.clear();

    // key的类型错误
    redis_set_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_set_test::m_pSet->sunion(keys, result));
}

