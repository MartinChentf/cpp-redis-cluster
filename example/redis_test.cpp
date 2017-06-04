#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>

#include "redis.h"

using namespace std;

void test()
{}

int main()
{
    std::string result;

    redis re("192.168.199.131", 10006);
    re.set("foo", "hello world!");
    re.get("foo", result);

    printf("result:[%s]\n", result.c_str());

    test();
}
