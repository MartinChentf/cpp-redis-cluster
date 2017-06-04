#include "redis_server.h"

redis_server::redis_server(const std::string & host, uint16_t port)
: redis_command(host, port)
{
}

bool redis_server::bgrewriteaof()
{
    std::vector<std::string> argv;
    argv.push_back("BGREWRITEAOF");

    sendCommand(argv);
    return check_status();
}

bool redis_server::flushall()
{
    std::vector<std::string> argv;
    argv.push_back("FLUSHALL");

    sendCommand(argv);
    return check_status();
}

