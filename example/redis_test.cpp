#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>

#include "redis_client.h"
#include "redis_string.h"
#include "redis_zset.h"
#include "redis_log.h"
#include "redis_server.h"
#include "socket_client.h"

using namespace std;

void test()
{}

int main()
{
    redis_client redis("192.168.199.131", 10006);
    redis_zset zset(&redis);

    std::vector<std::pair<std::string,double> > member_score;
    member_score.push_back(std::make_pair("a", 1));
    member_score.push_back(std::make_pair("b", 2));
    member_score.push_back(std::make_pair("c", 3));
    zset.zadd("{foo}:1", member_score);

    member_score.clear();
    member_score.push_back(std::make_pair("d", 4));
    member_score.push_back(std::make_pair("b", 2));
    member_score.push_back(std::make_pair("c", 3));
    zset.zadd("{foo}:2", member_score);

    std::vector<std::string> keys;
    keys.push_back("{foo}:1");
    keys.push_back("{foo}:2");

    zset.zinterstore("{foo}:0", keys);

    test();
}
