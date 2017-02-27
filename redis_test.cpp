#include <iostream>
#include "redis_client.h"
#include "redis_string.h"

using namespace std;

int main()
{
    redis_client redis1("10.45.4.201", 7000);
    redis_string str1(&redis1);

    str1.set("name", "martin");
    cout << str1.get("name") << endl;

    redis_client* predis = new redis_client("10.45.4.201", 7008)
    redis_string str2(predis);

    str2.set("name", "martin");
    cout << str2.get("name") << endl;
    
    
}
