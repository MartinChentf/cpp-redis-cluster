#include <iostream>
#include <string>
#include <string.h>

#include "redis_client.h"
#include "redis_string.h"
#include "redis_log.h"
#include "redis_server.h"
//#include "gt_common.h"
#include "socket_client.h"

using namespace std;

void test()
{
    char ret[1024*16];

    string buff("");
    buff += "*3\r\n";
    buff += "$3\r\n";
    buff += "SET\r\n";
    buff += "$3\r\n";
    buff += "foo\r\n";
    buff += "$4\r\n";
    buff += "mars\r\n";

    socket_client* sock = new socket_client();
    sock->connect_socket("192.168.199.131", 10006);
    sock->send_msg(buff.c_str());
    sock->recv_msg(ret, 1024*16);
    printf("return:[%s]", ret);

    delete sock;
}

int main()
{
    //redis_client redis(gt_component::Instance().get_host(),
    //                    gt_component::Instance().get_port());

    test();
}
