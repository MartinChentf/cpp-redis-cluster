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

TEST_F(redis_string_test, setnx)
{
    EXPECT_EQ(true, redis_string_test::m_pStr->setnx("foo", "world"));  // foo已存在未发生改变
    EXPECT_EQ(std::string("hello"), redis_string_test::m_pStr->get("foo"));
    EXPECT_EQ(true, redis_string_test::m_pStr->setnx("foo1", "world"));
    EXPECT_EQ(std::string("world"), redis_string_test::m_pStr->get("foo1"));

    redis_string_test::m_pKey->del("foo1");
}

TEST_F(redis_string_test, setex)
{
    EXPECT_EQ(true, redis_string_test::m_pStr->setex("foo", 5, "world"));
    sleep(3);
    EXPECT_EQ(std::string("world"), redis_string_test::m_pStr->get("foo")); // 3秒之后读取，foo仍然存在
    sleep(3);
    EXPECT_EQ(std::string(""), redis_string_test::m_pStr->get("foo"));  // 6秒之后读取，foo超时，不存在
}

TEST_F(redis_string_test, psetex)
{
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

