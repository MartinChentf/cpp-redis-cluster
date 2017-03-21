#include <vector>
#include <string>
#include <map>

#include "gt_redis_helper.h"
#include "redis_helper.h"

TEST_F(redis_helper_test, to_string)
{
    EXPECT_EQ(std::string("123456"), redis_helper::to_string(123456));    // int
    EXPECT_EQ(std::string("12.34"), redis_helper::to_string(12.34));    // double
    EXPECT_EQ(std::string("1234567890"), redis_helper::to_string(1234567890L));  // long
    EXPECT_EQ(std::string("1234567890"), redis_helper::to_string(1234567890UL));  // unsigned long
}

TEST_F(redis_helper_test, join_vector)
{
    std::vector<std::string> list;
    list.push_back("hello");
    list.push_back("redis");
    list.push_back("client!");

    EXPECT_EQ(std::string("hello redis client!"), redis_helper::join(list));
    EXPECT_EQ(std::string("hello-redis-client!"), redis_helper::join(list, "-"));
}

TEST_F(redis_helper_test, join_map)
{
    std::map<std::string, std::string> map;
    map["name"] = "martin";
    map["age"] = "27";
    map["height"] = "183";

    EXPECT_EQ(std::string("age:27,height:183,name:martin"), redis_helper::join(map, ":", ","));
}
