#include <string>

#include "gtest/gtest.h"
#include "gt_redis_string.h"

redis_client* redis_string_test::m_pClient = NULL;

TEST_F(redis_string_test, set) {
    redis_string str(redis_string_test::m_pClient);

    EXPECT_EQ(true, str.set("name", "martin"));
}

TEST_F(redis_string_test, get) {
    redis_string str(redis_string_test::m_pClient);

    EXPECT_EQ(std::string("martin"), str.get("name"));
}


