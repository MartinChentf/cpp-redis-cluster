#include <string>

#include "gtest/gtest.h"
#include "gt_redis_string.h"

#include "redis_client.h"
#include "redis_string.h"
#include "redis_key.h"

redis_client* redis_string_test::m_pClient = NULL;
redis_string* redis_string_test::m_pStr = NULL;
redis_key* redis_string_test::m_pKey = NULL;

void redis_string_test::SetUpTestCase() {
    m_pClient = new redis_client("192.168.199.131", 10000);
    m_pStr = new redis_string(m_pClient);
    m_pKey = new redis_key(m_pClient);
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
    EXPECT_EQ(true, redis_string_test::m_pStr->set("foo", "hello"));
    EXPECT_EQ(std::string("hello"), redis_string_test::m_pStr->get("foo"));
}

TEST_F(redis_string_test, getset) {
    EXPECT_EQ(std::string("hello"), redis_string_test::m_pStr->getSet("foo", "world"));
    EXPECT_EQ(std::string("world"), redis_string_test::m_pStr->get("foo"));
}

TEST_F(redis_string_test, setnx) {
    EXPECT_EQ(true, redis_string_test::m_pStr->setnx("foo", "world"));  // foo已存在未发生改变
    EXPECT_EQ(std::string("hello"), redis_string_test::m_pStr->get("foo"));
    EXPECT_EQ(true, redis_string_test::m_pStr->setnx("foo1", "world"));
    EXPECT_EQ(std::string("world"), redis_string_test::m_pStr->get("foo1"));

    redis_string_test::m_pKey->del("foo1");
}

TEST_F(redis_string_test, setex) {
    EXPECT_EQ(true, redis_string_test::m_pStr->setex("foo", 5, "world"));
    sleep(3);
    EXPECT_EQ(std::string("world"), redis_string_test::m_pStr->get("foo")); // 3秒之后读取，foo仍然存在
    sleep(3);
    EXPECT_EQ(std::string(""), redis_string_test::m_pStr->get("foo"));  // 6秒之后读取，foo超时，不存在
}

TEST_F(redis_string_test, psetex) {
    EXPECT_EQ(true, redis_string_test::m_pStr->psetex("foo", 5000, "world"));
    sleep(3);
    EXPECT_EQ(std::string("world"), redis_string_test::m_pStr->get("foo")); // 3000毫秒之后读取，foo仍然存在
    sleep(3);
    EXPECT_EQ(std::string(""), redis_string_test::m_pStr->get("foo"));  // 6000毫秒之后读取，foo超时，不存在
}

TEST_F(redis_string_test, getrange) {
    EXPECT_EQ(std::string("hell"), redis_string_test::m_pStr->getrange("foo", 0, 3));
}

TEST_F(redis_string_test, setrange) {
    EXPECT_EQ(10, redis_string_test::m_pStr->setrange("foo", 4, ",world"));
    EXPECT_EQ(std::string("hell,world"), redis_string_test::m_pStr->get("foo"));
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
    // ascii(h) = hex(68) = bin(01101000) -> bin(01001000) = hex(48) = ascii(H)
    EXPECT_EQ(1, redis_string_test::m_pStr->setbit("foo", 2, 0));
    EXPECT_EQ(std::string("Hello"), redis_string_test::m_pStr->get("foo"));

    // ascii(@) = hex(40) = bin(01000000)
    EXPECT_EQ(0, redis_string_test::m_pStr->setbit("foo", 41, 1));
    EXPECT_EQ(std::string("Hello@"), redis_string_test::m_pStr->get("foo")); // out of range
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

    EXPECT_EQ(5, redis_string_test::m_pStr->bitop(redis_string::AND, "foo", keys));
    EXPECT_EQ(std::string("\x40\x28\x40\x66\x14"), redis_string_test::m_pStr->get("foo")); // out of range

    redis_string_test::m_pKey->del("{foo}:1");
    redis_string_test::m_pKey->del("{foo}:2");
}

