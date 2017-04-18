#include "redis_server.h"

bool redis_server::bgrewriteaof()
{
    build_command("BGREWRITEAOF");
    return check_status();
}

bool redis_server::flushall()
{
    build_command("FLUSHALL");
    return check_status();
}

