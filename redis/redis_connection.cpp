#include "redis_connection.h"
#include "Util.h"

redis_connection::redis_connection(const std::string& host, uint16_t port)
: redis_command(host, port)
{
}

bool redis_connection::auth(const std::string password)
{
    std::vector<std::string> argv;
    argv.push_back("AUTH");
    argv.push_back(password);

    sendCommand(argv);

    return check_status();
}

std::string redis_connection::echo(const std::string message)
{
    std::vector<std::string> argv;
    argv.push_back("ECHO");
    argv.push_back(message);

    sendCommand(argv);

    std::string result;
    get_string(result);
    return result;
}

std::string redis_connection::ping(const std::string* message)
{
    std::vector<std::string> argv;
    argv.push_back("PING");
    if (message) {
        argv.push_back(*message);
    }

    sendCommand(argv);

    return get_status();
}

bool redis_connection::ping()
{
    std::vector<std::string> argv;
    argv.push_back("PING");

    sendCommand(argv);

    return check_status("PONG");
}

bool redis_connection::quit()
{
    std::vector<std::string> argv;
    argv.push_back("QUIT");

    sendCommand(argv);

    return check_status();
}

bool redis_connection::select(int index)
{
    std::vector<std::string> argv;
    argv.push_back("SELECT");
    argv.push_back(TO_STRING(index));

    sendCommand(argv);

    return check_status();
}

