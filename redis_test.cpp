#include <iostream>
#include "redis_client.h"
#include "redis_string.h"

using namespace std;

int main()
{
    redis_client redis("10.45.4.201", 7000);
    redis_string str(&redis);

    str.set("name", "martin");
    cout << str.get("name") << endl;
    
    
}
