#include <string>

#include "gtest/gtest.h"
#include "gt_redis_string.h"

redis_client* redis_string_test::m_pClient = NULL;

void redis_string_test::SetUp() {
    m_pStr = new redis_string(m_pClient);
    redis_string_test::m_pStr->set("name", "martin");
}

void redis_string_test::TearDown() {
    delete m_pStr;
}


TEST_F(redis_string_test, set_and_get) {
    EXPECT_EQ(true, redis_string_test::m_pStr->set("name", "martin"));
    EXPECT_EQ(std::string("martin"), redis_string_test::m_pStr->get("name"));
}

TEST_F(redis_string_test, getset) {
    EXPECT_EQ(std::string("martin"), redis_string_test::m_pStr->getSet("name", "mars"));
    EXPECT_EQ(std::string("mars"), redis_string_test::m_pStr->get("name"));
}

TEST_F(redis_string_test, getrange) {
    EXPECT_EQ(std::string("mart"), redis_string_test::m_pStr->getrange("name", 0, 3));
}

TEST_F(redis_string_test, setrange) {
    EXPECT_EQ(8, redis_string_test::m_pStr->setrange("name", 3, "schen"));
    EXPECT_EQ(std::string("marschen"), redis_string_test::m_pStr->get("name"));
}

