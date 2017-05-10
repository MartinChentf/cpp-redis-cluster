#include <string>

#include "gtest/gtest.h"
#include "gt_redis_string.h"
#include "gt_common.h"

#include "redis_client.h"
#include "redis_string.h"
#include "redis_key.h"

redis_client* redis_string_test::m_pClient = NULL;
redis_string* redis_string_test::m_pStr = NULL;
redis_key* redis_string_test::m_pKey = NULL;

void redis_string_test::SetUpTestCase() {
    m_pClient = new redis_client(gt_component::Instance().get_cluster_host(),
                                 gt_component::Instance().get_cluster_port());
    m_pStr = new redis_string(m_pClient);
    m_pKey = new redis_key(m_pClient);

    m_pKey->del("foo");
}

void redis_string_test::TearDownTestCase() {
    delete m_pClient;
    m_pClient = NULL;
    delete m_pStr;
    m_pStr = NULL;
    delete m_pKey;
    m_pKey = NULL;
}

void redis_string_test::SetUp() {
    m_pStr->set("foo", "hello");
}

void redis_string_test::TearDown() {
    m_pKey->del("foo");
}


TEST_F(redis_string_test, set_and_get) {
    std::string result;
    EXPECT_EQ(true, redis_string_test::m_pStr->set("foo", "hello"));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ("hello", result);
}

TEST_F(redis_string_test, getset) {
    std::string result;
    EXPECT_EQ(1, redis_string_test::m_pStr->getSet("foo", "world", result));
    EXPECT_EQ("hello", result);
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ("world", result);
}

TEST_F(redis_string_test, setnx) {
    std::string result;

    // foo已存在未发生改变
    EXPECT_EQ(false, redis_string_test::m_pStr->setnx("foo", "world"));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ("hello", result);

    EXPECT_EQ(true, redis_string_test::m_pStr->setnx("foo1", "world"));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo1", result));
    EXPECT_EQ("world", result);

    redis_string_test::m_pKey->del("foo1");
}

TEST_F(redis_string_test, setxx) {
    std::string result;

    // foo已存在发生改变
    EXPECT_EQ(true, redis_string_test::m_pStr->setxx("foo", "world"));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ("world", result);

    EXPECT_EQ(false, redis_string_test::m_pStr->setxx("foo1", "world"));
    EXPECT_EQ(0, redis_string_test::m_pStr->get("foo1", result));
}

TEST_F(redis_string_test, setex) {
    std::string result;

    EXPECT_EQ(true, redis_string_test::m_pStr->setex("foo", 5, "world"));

    sleep(3);
    // 3秒之后读取，foo仍然存在
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ("world", result);

    // 6秒之后读取，foo超时，不存在
    sleep(3);
    EXPECT_EQ(0, redis_string_test::m_pStr->get("foo", result));
}

TEST_F(redis_string_test, psetex) {
    std::string result;

    EXPECT_EQ(true, redis_string_test::m_pStr->psetex("foo", 5000, "world"));

    // 3000毫秒之后读取，foo仍然存在
    sleep(3);
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ("world", result);

    // 6000毫秒之后读取，foo超时，不存在
    sleep(3);
    EXPECT_EQ(0, redis_string_test::m_pStr->get("foo", result));
}

TEST_F(redis_string_test, getrange) {
    std::string result;
    EXPECT_EQ(true, redis_string_test::m_pStr->getrange("foo", 0, 3, result));
    EXPECT_EQ("hell", result);
}

TEST_F(redis_string_test, setrange) {
    long long length;
    std::string result;

    EXPECT_EQ(true, redis_string_test::m_pStr->setrange("foo", 4, ",world", &length));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ("hell,world", result);
}

TEST_F(redis_string_test, getbit) {
    // ascii:  h  e  l  l  o
    //   hex: 68 65 6c 6c 6f
    EXPECT_EQ(0, redis_string_test::m_pStr->getbit("foo", 0));  // ascii(h) = hex(68) = bin(01101000)
    EXPECT_EQ(1, redis_string_test::m_pStr->getbit("foo", 4));  // ascii(h) = hex(68) = bin(01101000)
    EXPECT_EQ(1, redis_string_test::m_pStr->getbit("foo", 10)); // ascii(e) = hex(65) = bin(01100101)
    EXPECT_EQ(0, redis_string_test::m_pStr->getbit("foo", 40)); // out of range
}

TEST_F(redis_string_test, setbit) {
    std::string result;

    // ascii(h) = hex(68) = bin(01101000) -> bin(01001000) = hex(48) = ascii(H)
    EXPECT_EQ(1, redis_string_test::m_pStr->setbit("foo", 2, 0));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ(std::string("Hello"), result);

    // ascii(@) = hex(40) = bin(01000000)
    EXPECT_EQ(0, redis_string_test::m_pStr->setbit("foo", 41, 1));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ("Hello@", result);
}

TEST_F(redis_string_test, bitcount) {
    // ascii:  h  e  l  l  o
    //   hex: 68 65 6c 6c 6f
    EXPECT_EQ(21, redis_string_test::m_pStr->bitconut("foo"));    
    EXPECT_EQ(21, redis_string_test::m_pStr->bitconut("foo", 0, -1)); // 表示统计整个字符串
    EXPECT_EQ(8, redis_string_test::m_pStr->bitconut("foo", 1, 2));
}

TEST_F(redis_string_test, bitop_AND) {
    std::vector<std::string> keys;
    keys.push_back("{foo}:1");
    keys.push_back("{foo}:2");
    redis_string_test::m_pStr->set("{foo}:1", "\xf0\x38\x72\x66\x54");
    redis_string_test::m_pStr->set("{foo}:2", "\x43\x2c\x41\x66\x14");

    std::string result;
    EXPECT_EQ(5, redis_string_test::m_pStr->bitop(redis_string::AND, "foo", keys));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ(0, memcmp("\x40\x28\x40\x66\x14", result.c_str(), 5));

    redis_string_test::m_pKey->del("{foo}:1");
    redis_string_test::m_pKey->del("{foo}:2");
}

TEST_F(redis_string_test, bitop_OR) {
    std::vector<std::string> keys;
    keys.push_back("{foo}:1");
    keys.push_back("{foo}:2");
    redis_string_test::m_pStr->set("{foo}:1", "\xf0\x38\x72\x66\x54");
    redis_string_test::m_pStr->set("{foo}:2", "\x43\x2c\x41\x66\x14\x0f");

    std::string result;
    EXPECT_EQ(6, redis_string_test::m_pStr->bitop(redis_string::OR, "foo", keys));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ(0, memcmp("\xf3\x3c\x73\x66\x54\x0f", result.c_str(), 6));

    redis_string_test::m_pKey->del("{foo}:1");
    redis_string_test::m_pKey->del("{foo}:2");
}

TEST_F(redis_string_test, bitop_NOT) {
    redis_string_test::m_pStr->set("{foo}:1", "\xf0\x38\x72\x66\x54");

    std::string result;
    EXPECT_EQ(5, redis_string_test::m_pStr->bitop(redis_string::NOT, "foo", "{foo}:1"));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ(0, memcmp("\x0f\xc7\x8d\x99\xab", result.c_str(), 5));

    redis_string_test::m_pKey->del("{foo}:1");
}

TEST_F(redis_string_test, bitop_XOR) {
    std::vector<std::string> keys;
    keys.push_back("{foo}:1");
    keys.push_back("{foo}:2");
    redis_string_test::m_pStr->set("{foo}:1", "\xf0\x38\x72\x66\x54");
    redis_string_test::m_pStr->set("{foo}:2", "\x43\x2c\x41\x66\x14\x0f");

    std::string result;
    EXPECT_EQ(6, redis_string_test::m_pStr->bitop(redis_string::XOR, "foo", keys));
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ(0, memcmp("\xb3\x14\x33\x00\x40\x0f", result.c_str(), 6));

    redis_string_test::m_pKey->del("{foo}:1");
    redis_string_test::m_pKey->del("{foo}:2");
}

TEST_F(redis_string_test, bitpos) {
    redis_string_test::m_pStr->set("foo", "\xf0\x38\x72\x66\x54");

    EXPECT_EQ(4, redis_string_test::m_pStr->bitpos("foo", 0));
    EXPECT_EQ(0, redis_string_test::m_pStr->bitpos("foo", 1));
    EXPECT_EQ(8, redis_string_test::m_pStr->bitpos("foo", 0, 1));
    EXPECT_EQ(17, redis_string_test::m_pStr->bitpos("foo", 1, 2, 3));
}

TEST_F(redis_string_test, mset_and_mget)
{
    std::map<std::string, std::string> key_val;
    key_val["{foo}:1"] = "hello";
    key_val["{foo}:2"] = "world";
    key_val["{foo}:3"] = "redis";

    // test mset
    EXPECT_EQ(true, redis_string_test::m_pStr->mset(key_val));

    std::vector<std::string> keys;
    keys.push_back("{foo}:1");
    keys.push_back("{foo}:2");
    keys.push_back("{foo}:3");
    keys.push_back("{foo}:4");
    std::vector<std::string*> values;

    EXPECT_EQ(true, redis_string_test::m_pStr->mget(keys, values));

    EXPECT_EQ("hello", *values[0]);
    EXPECT_EQ("world", *values[1]);
    EXPECT_EQ("redis", *values[2]);
    EXPECT_EQ(NULL, values[3]);

    for (size_t i = 0; i < values.size(); i ++) {
        delete values[i];
    }
    redis_string_test::m_pKey->del(keys);
}

TEST_F(redis_string_test, msetnx)
{
    std::map<std::string, std::string> key_val;
    key_val["{foo}:1"] = "hello";
    key_val["{foo}:2"] = "world";
    key_val["{foo}:3"] = "redis";
    key_val["foo"] = "hello";

    // 已经存在key = "foo"
    EXPECT_EQ(false, redis_string_test::m_pStr->msetnx(key_val));

    // 删除key = "foo", 再次设置
    key_val.erase("foo");
    EXPECT_EQ(true, redis_string_test::m_pStr->msetnx(key_val));

    std::map<std::string, std::string>::iterator it = key_val.begin();
    for (; it != key_val.end(); ++it) {
        redis_string_test::m_pKey->del(it->first);
    }
}

TEST_F(redis_string_test, incr)
{
    long long result;

    // value无法表示成整形数字
    EXPECT_EQ(false, redis_string_test::m_pStr->incr("foo", &result));
    redis_string_test::m_pStr->set("foo1", "2.5");
    EXPECT_EQ(false, redis_string_test::m_pStr->incr("foo1", &result));

    // value增加1
    redis_string_test::m_pStr->set("foo2", "12");
    EXPECT_EQ(true, redis_string_test::m_pStr->incr("foo2", &result));
    EXPECT_EQ(13, result);

    // 如果key不存在, 自增前将key的值设为0
    redis_string_test::m_pKey->del("foo2");
    EXPECT_EQ(true, redis_string_test::m_pStr->incr("foo2", &result));
    EXPECT_EQ(1, result);

    redis_string_test::m_pKey->del("foo1");    
    redis_string_test::m_pKey->del("foo2");
}

TEST_F(redis_string_test, incrby)
{
    long long result;

    // value无法表示成整形数字
    EXPECT_EQ(false, redis_string_test::m_pStr->incrby("foo", 2, &result));
    redis_string_test::m_pStr->set("foo1", "2.5");
    EXPECT_EQ(false, redis_string_test::m_pStr->incrby("foo1", 3, &result));

    // value增加12
    redis_string_test::m_pStr->set("foo2", "12");
    EXPECT_EQ(true, redis_string_test::m_pStr->incrby("foo2", 12, &result));
    EXPECT_EQ(24, result);

    // 如果key不存在, 自增前将key的值设为0
    redis_string_test::m_pKey->del("foo2");
    EXPECT_EQ(true, redis_string_test::m_pStr->incrby("foo2", 10, &result));
    EXPECT_EQ(10, result);

    redis_string_test::m_pKey->del("foo1");    
    redis_string_test::m_pKey->del("foo2");
}

TEST_F(redis_string_test, incrbyfloat)
{
    std::string result;

    // value无法表示成整形数字
    EXPECT_EQ(false, redis_string_test::m_pStr->incrbyfloat("foo", 2.5, &result));


    redis_string_test::m_pStr->set("foo1", "2.5");
    EXPECT_EQ(true, redis_string_test::m_pStr->incrbyfloat("foo1", 3.6, &result));
    EXPECT_EQ("6.1", result);
    redis_string_test::m_pStr->set("foo2", "12");
    EXPECT_EQ(true, redis_string_test::m_pStr->incrbyfloat("foo2", -4.2, &result));
    EXPECT_EQ("7.8", result);

    // 如果key不存在, 自增前将key的值设为0
    redis_string_test::m_pKey->del("foo2");
    EXPECT_EQ(true, redis_string_test::m_pStr->incrbyfloat("foo2", 2.5, &result));
    EXPECT_EQ("2.5", result);

    redis_string_test::m_pKey->del("foo1");    
    redis_string_test::m_pKey->del("foo2");
}

TEST_F(redis_string_test, decr)
{
    long long result;

    // value无法表示成整形数字
    EXPECT_EQ(false, redis_string_test::m_pStr->decr("foo", &result));
    redis_string_test::m_pStr->set("foo1", "2.5");
    EXPECT_EQ(false, redis_string_test::m_pStr->decr("foo1", &result));

    // value减一
    redis_string_test::m_pStr->set("foo2", "12");
    EXPECT_EQ(true, redis_string_test::m_pStr->decr("foo2", &result));
    EXPECT_EQ(11, result);

    // 如果key不存在, 自减前将key的值设为0
    redis_string_test::m_pKey->del("foo2");
    EXPECT_EQ(true, redis_string_test::m_pStr->decr("foo2", &result));
    EXPECT_EQ(-1, result);

    redis_string_test::m_pKey->del("foo1");    
    redis_string_test::m_pKey->del("foo2");
}

TEST_F(redis_string_test, decrby)
{
    long long result;

    // value无法表示成整形数字
    EXPECT_EQ(false, redis_string_test::m_pStr->decrby("foo", 2, &result));
    redis_string_test::m_pStr->set("foo1", "2.5");
    EXPECT_EQ(false, redis_string_test::m_pStr->decrby("foo1", 3, &result));

    // value减十
    redis_string_test::m_pStr->set("foo2", "12");
    EXPECT_EQ(true, redis_string_test::m_pStr->decrby("foo2", 10, &result));
    EXPECT_EQ(2, result);

    // 如果key不存在, 自减前将key的值设为0
    redis_string_test::m_pKey->del("foo2");
    EXPECT_EQ(true, redis_string_test::m_pStr->decrby("foo2", 10, &result));
    EXPECT_EQ(-10, result);

    redis_string_test::m_pKey->del("foo1");    
    redis_string_test::m_pKey->del("foo2");
}

TEST_F(redis_string_test, append)
{
    long long length = 0;
    std::string result = "";

    // key存在
    EXPECT_EQ(true, redis_string_test::m_pStr->append("foo", ",redis!", &length));
    EXPECT_EQ(12, length);
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo", result));
    EXPECT_EQ("hello,redis!", result);

    // key不存在
    redis_string_test::m_pKey->del("foo1"); // 防止存在foo1影响结果
    EXPECT_EQ(true, redis_string_test::m_pStr->append("foo1", ",redis!", &length));
    EXPECT_EQ(7, length);
    EXPECT_EQ(1, redis_string_test::m_pStr->get("foo1", result));
    EXPECT_EQ(",redis!", result);
}

TEST_F(redis_string_test, strlen)
{
    long long length = 0;

    // key存在
    EXPECT_EQ(true, redis_string_test::m_pStr->strlen("foo", length));
    EXPECT_EQ(5, length);

    // key不存在
    redis_string_test::m_pKey->del("foo1"); // 防止存在foo1影响结果
    EXPECT_EQ(true, redis_string_test::m_pStr->strlen("foo1", length));
    EXPECT_EQ(0, length);
}

