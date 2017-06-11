#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gt_redis_set.h"
#include "gt_common.h"

#include "Util.h"
#include "redis_log.h"
#include "redis.h"

redis* redis_set_test::m_pRedis = NULL;

void redis_set_test::SetUpTestCase() {
    m_pRedis = new redis(gt_component::Instance().get_host(0),
                         gt_component::Instance().get_port(0));

    m_pRedis->del("foo");
}

void redis_set_test::TearDownTestCase() {
    delete m_pRedis;
    m_pRedis = NULL;
}

void redis_set_test::SetUp()
{
    std::vector<std::string> member;
    member.push_back("one");
    member.push_back("two");
    member.push_back("three");

    m_pRedis->sadd("foo", member);
}

void redis_set_test::TearDown()
{
    m_pRedis->del("foo");
}

bool redis_set_test::contain_with(std::string str, std::vector<std::string>& search)
{
    bool iret = true;
    for (size_t i = 0; i < search.size(); i++) {
        if (!strstr(str.c_str(), search[i].c_str())) {
            iret = false;
            break;
        }
    }
    return iret;
}

TEST_F(redis_set_test, smembers)
{
    std::vector<std::string> result;

    // 获取集合成员
    EXPECT_EQ(3, redis_set_test::m_pRedis->smembers("foo", result));
    EXPECT_EQ(true, contain_with("one, two, three", result));

    // key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->smembers("foo", result));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->smembers("foo", result));
}

TEST_F(redis_set_test, sismember)
{
    // 判断指定成员是否存在
    EXPECT_EQ(1, redis_set_test::m_pRedis->sismember("foo", "one"));
    EXPECT_EQ(1, redis_set_test::m_pRedis->sismember("foo", "two"));
    EXPECT_EQ(0, redis_set_test::m_pRedis->sismember("foo", "four"));

    // key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->sismember("foo", "one"));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->sismember("foo", "one"));
}

TEST_F(redis_set_test, sadd)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");

    // 添加新成员
    EXPECT_EQ(1, redis_set_test::m_pRedis->sadd("foo", member));
    EXPECT_EQ(1, redis_set_test::m_pRedis->sismember("foo", "four"));
    EXPECT_EQ(1, redis_set_test::m_pRedis->sismember("foo", "two"));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->sadd("foo", member));
}

TEST_F(redis_set_test, scard)
{
    // 返回集合成员基数
    EXPECT_EQ(3, redis_set_test::m_pRedis->scard("foo"));

    // key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->scard("foo"));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->scard("foo"));
}

TEST_F(redis_set_test, sdiff)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pRedis->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // 返回集合之间差集
    EXPECT_EQ(true, redis_set_test::m_pRedis->sdiff(keys, result));
    EXPECT_EQ(true, contain_with("one, three", result));
    result.clear();

    // 后面的key不存在
    redis_set_test::m_pRedis->del("{foo}:1");
    EXPECT_EQ(true, redis_set_test::m_pRedis->sdiff(keys, result));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // 第一个key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(true, redis_set_test::m_pRedis->sdiff(keys, result));
    EXPECT_EQ("", Util::join(result, ", "));
    result.clear();

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_set_test::m_pRedis->sdiff(keys, result));

    redis_set_test::m_pRedis->del("{foo}:1");
}

TEST_F(redis_set_test, sinter)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pRedis->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // 返回集合之间交集
    EXPECT_EQ(true, redis_set_test::m_pRedis->sinter(keys, result));
    EXPECT_EQ("two", Util::join(result, ", "));
    result.clear();

    // 有的key为空
    redis_set_test::m_pRedis->del("{foo}:1");
    EXPECT_EQ(true, redis_set_test::m_pRedis->sinter(keys, result));
    EXPECT_EQ("", Util::join(result, ", "));
    result.clear();

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_set_test::m_pRedis->sinter(keys, result));

    redis_set_test::m_pRedis->del("{foo}:1");
}

TEST_F(redis_set_test, sunion)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pRedis->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // 返回集合之间并集
    EXPECT_EQ(true, redis_set_test::m_pRedis->sunion(keys, result));
    EXPECT_EQ(true, contain_with("one, two, three, four", result));
    result.clear();

    // 有的key为空
    redis_set_test::m_pRedis->del("{foo}:1");
    EXPECT_EQ(true, redis_set_test::m_pRedis->sunion(keys, result));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(false, redis_set_test::m_pRedis->sunion(keys, result));

    redis_set_test::m_pRedis->del("{foo}:1");
}

TEST_F(redis_set_test, sdiffstore)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pRedis->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // 返回集合之间差集
    EXPECT_EQ(2, redis_set_test::m_pRedis->sdiffstore("{foo}:2", keys));
    EXPECT_EQ(2, redis_set_test::m_pRedis->smembers("{foo}:2", result));
    EXPECT_EQ(true, contain_with("one, three", result));
    result.clear();

    // 后面的key不存在
    redis_set_test::m_pRedis->del("{foo}:1");
    EXPECT_EQ(3, redis_set_test::m_pRedis->sdiffstore("{foo}:2", keys));
    EXPECT_EQ(3, redis_set_test::m_pRedis->smembers("{foo}:2", result));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // 第一个key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->sdiffstore("{foo}:2", keys));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->sdiffstore("{foo}:2", keys));

    redis_set_test::m_pRedis->del("{foo}:1");
    redis_set_test::m_pRedis->del("{foo}:2");
}

TEST_F(redis_set_test, sinterstore)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pRedis->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // 返回集合之间交集
    EXPECT_EQ(1, redis_set_test::m_pRedis->sinterstore("{foo}:2", keys));
    EXPECT_EQ(1, redis_set_test::m_pRedis->smembers("{foo}:2", result));
    EXPECT_EQ("two", Util::join(result, ", "));
    result.clear();

    // 有的key为空
    redis_set_test::m_pRedis->del("{foo}:1");
    EXPECT_EQ(0, redis_set_test::m_pRedis->sinterstore("{foo}:2", keys));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->sinterstore("{foo}:2", keys));

    redis_set_test::m_pRedis->del("{foo}:1");
    redis_set_test::m_pRedis->del("{foo}:2");
}

TEST_F(redis_set_test, sunionstore)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");
    redis_set_test::m_pRedis->sadd("{foo}:1", member);

    std::vector<std::string> result;
    std::vector<std::string> keys;
    keys.push_back("foo");
    keys.push_back("{foo}:1");

    // 返回集合之间并集
    EXPECT_EQ(4, redis_set_test::m_pRedis->sunionstore("{foo}:2", keys));
    EXPECT_EQ(4, redis_set_test::m_pRedis->smembers("{foo}:2", result));
    EXPECT_EQ(true, contain_with("one, two, three, four", result));
    result.clear();

    // 有的key为空
    redis_set_test::m_pRedis->del("{foo}:1");
    EXPECT_EQ(3, redis_set_test::m_pRedis->sunionstore("{foo}:2", keys));
    EXPECT_EQ(3, redis_set_test::m_pRedis->smembers("{foo}:2", result));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->sunionstore("{foo}:2", keys));

    redis_set_test::m_pRedis->del("{foo}:1");
    redis_set_test::m_pRedis->del("{foo}:2");
}

TEST_F(redis_set_test, smove)
{
    // 移动指定元素
    EXPECT_EQ(1, redis_set_test::m_pRedis->smove("foo", "{foo}:1", "two"));
    EXPECT_EQ(0, redis_set_test::m_pRedis->sismember("foo", "two"));
    EXPECT_EQ(1, redis_set_test::m_pRedis->sismember("{foo}:1", "two"));

    // 指定元素不存在
    EXPECT_EQ(0, redis_set_test::m_pRedis->smove("foo", "{foo}:1", "two"));

    // src不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->smove("foo", "{foo}:1", "two"));

    // dest的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->smove("foo", "{foo}:1", "one"));

    redis_set_test::m_pRedis->del("{foo}:1");
}

TEST_F(redis_set_test, spop)
{
    std::string result;

    // 随机弹出一个元素
    EXPECT_EQ(1, redis_set_test::m_pRedis->spop("foo", result));

    // key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->spop("foo", result));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->spop("foo", result));
}

TEST_F(redis_set_test, spop_with_count)
{
    std::vector<std::string> result;

    // 随机弹出count个元素
    EXPECT_EQ(2, redis_set_test::m_pRedis->spop("foo", result, 2));

    // key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->spop("foo", result, 2));

    // count为负值
    EXPECT_EQ(-1, redis_set_test::m_pRedis->spop("foo", result, -2));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->spop("foo", result, 2));
}

TEST_F(redis_set_test, srandmember)
{
    std::string result;

    // 随机返回一个元素
    EXPECT_EQ(1, redis_set_test::m_pRedis->srandmember("foo", result));

    // key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->srandmember("foo", result));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->srandmember("foo", result));
}

TEST_F(redis_set_test, srandmember_with_count)
{
    std::vector<std::string> result;

    // 随机返回count个元素
    EXPECT_EQ(2, redis_set_test::m_pRedis->srandmember("foo", result, 2));
    result.clear();
    
    // 返回所有元素(count大于元素个数)
    EXPECT_EQ(3, redis_set_test::m_pRedis->srandmember("foo", result, 10));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // count为负值
    EXPECT_EQ(5, redis_set_test::m_pRedis->srandmember("foo", result, -5));
    EXPECT_EQ(true, contain_with("one, two, three", result));
    result.clear();

    // count = 0
    EXPECT_EQ(0, redis_set_test::m_pRedis->srandmember("foo", result, 0));

    // key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->srandmember("foo", result, 2));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->srandmember("foo", result, 2));
}

TEST_F(redis_set_test, srem)
{
    std::vector<std::string> member;
    member.push_back("four");
    member.push_back("two");

    // 移除指定元素
    EXPECT_EQ(1, redis_set_test::m_pRedis->srem("foo", member));
    EXPECT_EQ(0, redis_set_test::m_pRedis->sismember("foo", "two"));

    // 指定元素不存在
    EXPECT_EQ(0, redis_set_test::m_pRedis->srem("foo", member));

    // key不存在
    redis_set_test::m_pRedis->del("foo");
    EXPECT_EQ(0, redis_set_test::m_pRedis->srem("foo", member));

    // key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->srem("foo", member));
}

TEST_F(redis_set_test, sscan)
{
    std::vector<std::string> member;
    for (int i = 0; i < 26; i++) {
        char str[3] = {0};
        snprintf(str, sizeof(str), "%c%c", i + 'a', i + 'a');
        member.push_back(str);
    }
    redis_set_test::m_pRedis->sadd("foo", member);

    std::vector<std::string> result;
    // 1. 遍历
    int cursor = 0;
    do {
        cursor = redis_set_test::m_pRedis->sscan("foo", cursor, result);
    } while (cursor > 0);
    EXPECT_EQ(29, result.size());
    std::string strTest = "one, two, three, " + Util::join(result, ", ");
    EXPECT_EQ(true, contain_with(strTest, result));
    result.clear();

    // 2. 使用模式参数
    cursor = 0;
    do {
        cursor = redis_set_test::m_pRedis->sscan("foo", cursor, result, "*e*");
    } while (cursor > 0);
    EXPECT_EQ(3, result.size());
    DEBUG("%s", Util::join(result, ", ").c_str());
    EXPECT_EQ(true, contain_with("one, three, ee", result));
    result.clear();

    // 3. key的类型错误
    redis_set_test::m_pRedis->set("foo", "hello");
    EXPECT_EQ(-1, redis_set_test::m_pRedis->sscan("foo", 0, result, "*e*"));
}

