#include "gtest/gtest.h"
#include "gt_redis_hash.h"
#include "gt_common.h"

#include "redis_client.h"
#include "redis_hash.h"
#include "redis_key.h"
#include "redis_helper.h"
#include "redis_string.h"

redis_client* redis_hash_test::m_pClient = NULL;
redis_string* redis_hash_test::m_pStr = NULL;
redis_hash* redis_hash_test::m_pHash= NULL;
redis_key* redis_hash_test::m_pKey = NULL;

void redis_hash_test::SetUpTestCase() {
    m_pClient = new redis_client(gt_component::Instance().get_host(),
                                 gt_component::Instance().get_port());
    m_pHash = new redis_hash(m_pClient);
    m_pKey = new redis_key(m_pClient);
    m_pStr = new redis_string(m_pClient);
}

void redis_hash_test::TearDownTestCase() {
    delete m_pClient;
    m_pClient = NULL;
    delete m_pHash;
    m_pHash = NULL;
    delete m_pKey;
    m_pKey = NULL;
}

void redis_hash_test::SetUp()
{
    std::map<std::string, std::string> field_value;
    field_value["f1"] = "a";
    field_value["f2"] = "b";
    field_value["f3"] = "c";
    field_value["f4"] = "d";
    field_value["f5"] = "e";

    redis_hash_test::m_pHash->hmset("foo", field_value);
}

void redis_hash_test::TearDown()
{
    redis_hash_test::m_pKey->del("foo");
}

TEST_F(redis_hash_test, hgetall)
{
    std::map<std::string,std::string> result;

    EXPECT_EQ(true, redis_hash_test::m_pHash->hgetall("foo", result));
    EXPECT_EQ("f1 a f2 b f3 c f4 d f5 e", redis_helper::join(result));

    // key类型错误
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_hash_test::m_pHash->hgetall("foo", result));
}

TEST_F(redis_hash_test, hmset)
{
    std::map<std::string,std::string> result;

    std::map<std::string, std::string> field_value;
    field_value["f1"] = "aaa";
    field_value["f6"] = "f";

    // 1. 更新field值/增加新的field
    EXPECT_EQ(true, redis_hash_test::m_pHash->hmset("foo", field_value));
    redis_hash_test::m_pHash->hgetall("foo", result);
    EXPECT_EQ("f1 aaa f2 b f3 c f4 d f5 e f6 f", redis_helper::join(result));
    result.clear();

    // 2. key不存在的情况
    redis_hash_test::m_pKey->del("foo");
    EXPECT_EQ(true, redis_hash_test::m_pHash->hmset("foo", field_value));
    redis_hash_test::m_pHash->hgetall("foo", result);
    EXPECT_EQ("f1 aaa f6 f", redis_helper::join(result));
    result.clear();

    // 3. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_hash_test::m_pHash->hmset("foo", field_value));
}

TEST_F(redis_hash_test, hdel)
{
    std::vector<std::string> fields;
    fields.push_back("f1");
    fields.push_back("f3");
    fields.push_back("f5");
    fields.push_back("f7");
    // 删除指定域
    EXPECT_EQ(3, redis_hash_test::m_pHash->hdel("foo", fields));

    // 查看删除后字段
    std::map<std::string,std::string> result;
    redis_hash_test::m_pHash->hgetall("foo", result);
    EXPECT_EQ("f2 b f4 d", redis_helper::join(result));
}

TEST_F(redis_hash_test, hexists)
{
    // 1. field存在的情况
    EXPECT_EQ(1, redis_hash_test::m_pHash->hexists("foo", "f2"));

    // 2. field不存在的情况
    EXPECT_EQ(0, redis_hash_test::m_pHash->hexists("foo", "f6"));

    // 3. key不存在的情况
    redis_hash_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_hash_test::m_pHash->hexists("foo", "f2"));

    // 4. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_hash_test::m_pHash->hexists("foo", "f2"));
}

TEST_F(redis_hash_test, hget)
{
    std::string result;

    // 1. field存在的情况
    EXPECT_EQ(1, redis_hash_test::m_pHash->hget("foo", "f2", result));
    EXPECT_EQ("b", result);

    // 2. field不存在的情况
    EXPECT_EQ(0, redis_hash_test::m_pHash->hget("foo", "f6", result));

    // 3. key不存在的情况
    redis_hash_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_hash_test::m_pHash->hget("foo", "f2", result));

    // 4. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_hash_test::m_pHash->hget("foo", "f2", result));
}

TEST_F(redis_hash_test, hset)
{
    std::string result;

    // 1. field存在更新其值
    EXPECT_EQ(0, redis_hash_test::m_pHash->hset("foo", "f2", "bbb"));
    EXPECT_EQ(1, redis_hash_test::m_pHash->hget("foo", "f2", result));
    EXPECT_EQ("bbb", result);

    // 2. field不存在, 插入新值
    EXPECT_EQ(1, redis_hash_test::m_pHash->hset("foo", "f6", "fff"));
    EXPECT_EQ(1, redis_hash_test::m_pHash->hget("foo", "f6", result));
    EXPECT_EQ("fff", result);

    // 3. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_hash_test::m_pHash->hset("foo", "f2", "bbb"));
}

TEST_F(redis_hash_test, hsetnx)
{
    std::string result;

    // 1. field存在, 不执行操作
    EXPECT_EQ(0, redis_hash_test::m_pHash->hsetnx("foo", "f2", "bbb"));
    EXPECT_EQ(1, redis_hash_test::m_pHash->hget("foo", "f2", result));
    EXPECT_EQ("b", result);

    // 2. field不存在, 插入新值
    EXPECT_EQ(1, redis_hash_test::m_pHash->hsetnx("foo", "f6", "fff"));
    EXPECT_EQ(1, redis_hash_test::m_pHash->hget("foo", "f6", result));
    EXPECT_EQ("fff", result);

    // 3. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_hash_test::m_pHash->hsetnx("foo", "f2", "bbb"));
}

TEST_F(redis_hash_test, hincrby)
{
    long long result;

    // 1. field关联的值是不能表示成数字的字符串
    EXPECT_EQ(false, redis_hash_test::m_pHash->hincrby("foo", "f2", 2, &result));

    // 2. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_hash_test::m_pHash->hincrby("foo", "f2", 2, &result));

    // 3. key不存在的情况
    redis_hash_test::m_pKey->del("foo");
    EXPECT_EQ(true, redis_hash_test::m_pHash->hincrby("foo", "f2", 2, &result));
    EXPECT_EQ(2, result);

    // 5. field存在的情况
    EXPECT_EQ(true, redis_hash_test::m_pHash->hincrby("foo", "f2", 2, &result));
    EXPECT_EQ(4, result);

    // 4. field不存在的情况
    EXPECT_EQ(true, redis_hash_test::m_pHash->hincrby("foo", "f3", 3, &result));
    EXPECT_EQ(3, result);
}

TEST_F(redis_hash_test, hincrbyfloat)
{
    std::string result;

    // 1. field关联的值不能解析成双精度浮点值的字符串
    EXPECT_EQ(false, redis_hash_test::m_pHash->hincrbyfloat("foo", "f2", 2.5, &result));

    // 2. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_hash_test::m_pHash->hincrbyfloat("foo", "f2", 2.5, &result));

    // 3. key不存在的情况
    redis_hash_test::m_pKey->del("foo");
    EXPECT_EQ(true, redis_hash_test::m_pHash->hincrbyfloat("foo", "f2", 2.5, &result));
    EXPECT_EQ("2.5", result);

    // 5. field存在的情况
    EXPECT_EQ(true, redis_hash_test::m_pHash->hincrbyfloat("foo", "f2", 3.4, &result));
    EXPECT_EQ("5.9", result);

    // 4. field不存在的情况
    EXPECT_EQ(true, redis_hash_test::m_pHash->hincrbyfloat("foo", "f3", 3.8, &result));
    EXPECT_EQ("3.8", result);
}

TEST_F(redis_hash_test, hkeys)
{
    std::vector<std::string> result;

    // 1. key存在的情况
    EXPECT_EQ(true, redis_hash_test::m_pHash->hkeys("foo", result));
    EXPECT_EQ("f1 f2 f3 f4 f5", redis_helper::join(result));
    result.clear();

    // 2. key不存在的情况
    redis_hash_test::m_pKey->del("foo");
    EXPECT_EQ(true, redis_hash_test::m_pHash->hkeys("foo", result));
    EXPECT_EQ("", redis_helper::join(result));

    // 3. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_hash_test::m_pHash->hkeys("foo", result));
}

TEST_F(redis_hash_test, hvals)
{
    std::vector<std::string> result;

    // 1. key存在的情况
    EXPECT_EQ(true, redis_hash_test::m_pHash->hvals("foo", result));
    EXPECT_EQ("a b c d e", redis_helper::join(result));
    result.clear();

    // 2. key不存在的情况
    redis_hash_test::m_pKey->del("foo");
    EXPECT_EQ(true, redis_hash_test::m_pHash->hvals("foo", result));
    EXPECT_EQ("", redis_helper::join(result));

    // 3. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_hash_test::m_pHash->hvals("foo", result));
}

TEST_F(redis_hash_test, hlen)
{
    EXPECT_EQ(5, redis_hash_test::m_pHash->hlen("foo"));

    // 1. key不存在的情况
    redis_hash_test::m_pKey->del("foo");
    EXPECT_EQ(0, redis_hash_test::m_pHash->hlen("foo"));

    // 2. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_hash_test::m_pHash->hlen("foo"));
}

TEST_F(redis_hash_test, hmget)
{
    std::vector<std::string*> result;
    std::vector<std::string> fields;
    fields.push_back("f1");
    fields.push_back("f6");
    fields.push_back("f4");

    // 1. field存在和不存在的情况
    EXPECT_EQ(true, redis_hash_test::m_pHash->hmget("foo", fields, result));
    EXPECT_EQ("a", *result[0]);
    EXPECT_EQ(NULL, result[1]);
    EXPECT_EQ("d", *result[2]);
    for (size_t i = 0; i < result.size(); ++i) {
        delete result[i];
    }
    result.clear();
    
    // 2. key不存在
    EXPECT_EQ(true, redis_hash_test::m_pHash->hmget("foo1", fields, result));
    EXPECT_EQ(NULL, result[0]);
    EXPECT_EQ(NULL, result[1]);    
    EXPECT_EQ(NULL, result[2]);

    // 3. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(false, redis_hash_test::m_pHash->hmget("foo", fields, result));
}

TEST_F(redis_hash_test, hscan)
{
    std::map<std::string,std::string> result;

    // 1. 遍历结束(因为哈希表比较小, 所以一次调用即可遍历整个哈希表)
    EXPECT_EQ(0, redis_hash_test::m_pHash->hscan("foo", 0, result));
    EXPECT_EQ("f1 a f2 b f3 c f4 d f5 e", redis_helper::join(result));
    result.clear();

    // 2 使用模式参数
    redis_hash_test::m_pHash->hset("foo", "f12", "ab");
    EXPECT_EQ(0, redis_hash_test::m_pHash->hscan("foo", 0, result, "*2"));
    EXPECT_EQ("f12 ab f2 b", redis_helper::join(result));
    result.clear();

    // 3. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_hash_test::m_pHash->hscan("foo", 0, result));
}

TEST_F(redis_hash_test, hstrlen)
{
    // 1. field域存在
    redis_hash_test::m_pHash->hset("foo", "f2", "hello");
    EXPECT_EQ(5, redis_hash_test::m_pHash->hstrlen("foo", "f2"));

    // 2. field域不存在
    EXPECT_EQ(0, redis_hash_test::m_pHash->hstrlen("foo", "f6"));

    // 3. key类型错误的情况
    redis_hash_test::m_pStr->set("foo", "hello");
    EXPECT_EQ(-1, redis_hash_test::m_pHash->hstrlen("foo", "f2"));
}

