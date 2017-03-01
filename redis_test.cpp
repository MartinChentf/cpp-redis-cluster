#include <iostream>
#include <string>

#include "redis_client.h"
#include "redis_string.h"
#include "redis_log.h"
#include "redis_server.h"

using namespace std;

void test(redis_client* client)
{
    redis_string str(client);
    string key("name");

    cout << PRINT_COLOR_PURPLE "-------------------------------------------------------------" PRINT_COLOR_NONE<< endl;
    cout << "[set name martin]:" << str.set(key, "martin") << endl;
    cout << "[get name]:" << str.get(key) << endl;
    cout << PRINT_COLOR_PURPLE "-------------------------------------------------------------" PRINT_COLOR_NONE<< endl;

    cout << "[getrange 0 3]:" << str.getrange(key, 0, 3) << endl;
    cout << "[setrange name 3 s---]:" << str.setrange(key, 3, "chen") << endl;
    cout << PRINT_COLOR_PURPLE "-------------------------------------------------------------" PRINT_COLOR_NONE<< endl;

    cout << "[getset name mars]:" << str.getSet(key, "mars") << endl;
    cout << "[get name]:" << str.get(key) << endl;
    cout << PRINT_COLOR_PURPLE "-------------------------------------------------------------" PRINT_COLOR_NONE<< endl;

    long long bit = str.getbit(key, 0);
    cout << "[getbit name 0]:" << bit << endl;
    cout << "[setbit name 0]:" << str.setbit(key, 0, bit == 0 ? 1 : 0) << endl;
    cout << "[get name]:" << str.get(key) << endl;
    cout << "[setbit name 0]:" << str.setbit(key, 0, bit) << endl;
    cout << PRINT_COLOR_PURPLE "-------------------------------------------------------------" PRINT_COLOR_NONE<< endl;

    map<string, string> key_value;
    key_value["red"] = "redd";
    key_value["green"] = "green";
    key_value["blue"] = "blue";
    cout << "[mset key value ...]:" << str.mset(key_value) << endl;

    vector<string> keys;
    vector<string> values;
    keys.push_back("red");
    keys.push_back("yellow");
    keys.push_back("blue");
    cout << "[mget key ...]:" << str.mget(keys, values) << endl;
    vector<string>::iterator it = values.begin();
    for (; it != values.end(); ++ it) {
        cout << (*it) << " ";
    }
    cout << endl;
    cout << PRINT_COLOR_PURPLE "-------------------------------------------------------------" PRINT_COLOR_NONE<< endl;
    redis_server ser(client);
    ser.flushall();
}

int main()
{
#if 0
    redis_client redis1("10.45.4.201", 7000);
    redis_client redis2("10.45.4.201", 7008);
#else
    redis_client redis1("192.168.199.131", 10000);
    redis_client redis2("192.168.199.131", 10006);
#endif

    test(&redis1);
    test(&redis2);
}
