#include <string>

#include "gtest/gtest.h"
#include "gt_redis_key.h"
#include "gt_common.h"

#include "redis_client.h"
#include "redis_string.h"
#include "redis_key.h"
#include "redis_helper.h"

redis_client* redis_key_test::m_pClient = NULL;
redis_string* redis_key_test::m_pStr = NULL;
redis_key* redis_key_test::m_pKey = NULL;

void redis_key_test::SetUpTestCase() {
    m_pClient = new redis_client(gt_component::Instance().get_host(),
                                 gt_component::Instance().get_port());
    m_pStr = new redis_string(m_pClient);
    m_pKey = new redis_key(m_pClient);

    m_pKey->del("foo");
}

void redis_key_test::TearDownTestCase() {
    delete m_pClient;
    m_pClient = NULL;
    delete m_pStr;
    m_pStr = NULL;
    delete m_pKey;
    m_pKey = NULL;
}

void redis_key_test::SetUp() {
    m_pStr->set("foo", "hello");
}

void redis_key_test::TearDown() {
    m_pKey->del("foo");
}

TEST_F(redis_key_test, del)
{
    std::string result;

    // 删除一个key
    EXPECT_EQ(1, redis_key_test::m_pKey->del("foo"));
    EXPECT_EQ(0, redis_key_test::m_pStr->get("foo", result));

    std::vector<std::string*> results;
    std::vector<std::string> keys;
    std::vector<std::string> keys_del;
    for (int i = 1; i <= 10; i++) {
        std::string key("{foo}:");
        key += TO_STRING(i);
        redis_key_test::m_pStr->set(key, TO_STRING(i));
        keys.push_back(key);
        if (i%2) keys_del.push_back(key);
    }

    // 删除多个key
    EXPECT_EQ(5, redis_key_test::m_pKey->del(keys_del));
    EXPECT_EQ(true, redis_key_test::m_pStr->mget(keys, results));
    EXPECT_EQ(",2,4,6,8,10",
              redis_helper::join(results, ","));
    results.clear();

    // 未删除key
    EXPECT_EQ(0, redis_key_test::m_pKey->del(keys_del));

    // 删除部分key
    std::vector<std::string> other_keys;
    other_keys.push_back("{foo}:1");
    other_keys.push_back("{foo}:2");
    other_keys.push_back("{foo}:3");
    other_keys.push_back("{foo}:4");
    EXPECT_EQ(2, redis_key_test::m_pKey->del(other_keys));
    EXPECT_EQ(true, redis_key_test::m_pStr->mget(keys, results));
    EXPECT_EQ(",,,,,6,8,10", redis_helper::join(results, ","));
    results.clear();

    redis_key_test::m_pKey->del(keys);
}

