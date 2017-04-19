#include "redis_server.h"

bool redis_server::bgrewriteaof()
{
    //build_command("BGREWRITEAOF");
    std::vector<std::string> argv;
    argv.push_back("BGREWRITEAOF");

    return check_status();
}

bool redis_server::flushall()
{
    //build_command("FLUSHALL");
    std::vector<std::string> argv;
    argv.push_back("FLUSHALL");
    return check_status();
}

