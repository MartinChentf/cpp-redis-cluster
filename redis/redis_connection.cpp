#include "redis_connection.h"
#include "redis_helper.h"

redis_connection::redis_connection(const std::string& host, uint16_t port)
: redis_command(host, port)
{
}

bool redis_connection::select(int index)
{
    std::vector<std::string> argv;
    argv.push_back("SELECT");
    argv.push_back(TO_STRING(index));

    sendCommand(argv);

    return check_status();
}

