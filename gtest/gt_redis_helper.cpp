#include <vector>
#include <string>
#include <map>

#include "gt_redis_helper.h"
#include "Util.h"

TEST_F(redis_helper_test, to_string)
{
    EXPECT_EQ(std::string("123456"), Util::to_string(123456));  // int
    EXPECT_EQ(std::string("12.34"), Util::to_string(12.34));    // double
    EXPECT_EQ(std::string("1234567890"), Util::to_string(1234567890L)); // long
    EXPECT_EQ(std::string("1234567890"), Util::to_string(1234567890UL));// unsigned long
    EXPECT_EQ(std::string("name:martin"),
        Util::to_string(std::make_pair("name", "martin"))); // std::pair
}

TEST_F(redis_helper_test, join_vector)
{
    std::vector<std::string> list;
    list.push_back("hello");
    list.push_back("redis");
    list.push_back("client!");

    EXPECT_EQ(std::string("hello redis client!"), Util::join(list));
    EXPECT_EQ(std::string("hello-redis-client!"), Util::join(list, "-"));
}

TEST_F(redis_helper_test, join_map)
{
    std::map<std::string, double> map;
    map["foo1"] = 11;
    map["foo2"] = 22;
    map["foo3"] = 33;

    EXPECT_EQ(std::string("foo1:11,foo2:22,foo3:33"), Util::join(map, ":", ","));
}
