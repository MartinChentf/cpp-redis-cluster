#include "redis_server.h"
#include "Util.h"

const char* TYPE_STR[] = { TYPE_NORMAL, TYPE_MASTER, TYPE_SLAVE, TYPE_PUBSUB };

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

std::string redis_server::bgsave()
{
    std::vector<std::string> argv;
    argv.push_back("BGSAVE");

    sendCommand(argv);
    return get_string();
}

bool redis_server::clientGetname(std::string & name)
{
    std::vector<std::string> argv;
    argv.push_back("CLIENT");
    argv.push_back("GETNAME");

    sendCommand(argv);
    return get_string(name);
}

bool redis_server::clientKill(const std::string & addr)
{
    std::vector<std::string> argv;
    argv.push_back("CLIENT");
    argv.push_back("KILL");
    argv.push_back(addr);

    sendCommand(argv);
    return check_status();
}

int redis_server::clientKill(KillFilterParams params)
{
    std::vector<std::string> argv;
    argv.push_back("CLIENT");
    argv.push_back("KILL");
    argv.inster(argv.end(), params.getParams().begin(),
        params.getParams().end());

    sendCommand(argv);
    return get_integer64();
}

std::string redis_server::clientList(
    std::vector<std::map<std::string,std::string>*>* list /*= NULL*/)
{
    typedef std::map<std::string,std::string> PropertyMap;

    std::vector<std::string> argv;
    argv.push_back("CLIENT");
    argv.push_back("LIST");

    sendCommand(argv);
    std::string ret = get_string();

    if (list) {
        std::vector<std::string> client;
        Util::split(ret, "\n", client);

        for (int i = 0; i < client.size(); i++) {
            std::vector<std::string> property;
            Util::split(client[i], " ", property);

            PropertyMap* map = new PropertyMap();
            for (int j = 0; j < property.size(); j++) {
                std::vector<std::string> item;
                Util::split(property[i], "=", item);

                if (item.size() == 2) {
                    (*map)[item[0]] = item[1];
                }
            }
            list.push_back(map);
        }
    }

    return ret;
}

bool redis_server::flushAll()
{
    std::vector<std::string> argv;
    argv.push_back("FLUSHALL");

    sendCommand(argv);
    return check_status();
}

bool redis_server::flushDB()
{
    std::vector<std::string> argv;
    argv.push_back("FLUSHDB");

    sendCommand(argv);
    return check_status();
}

long long redis_server::lastsave()
{
    std::vector<std::string> argv;
    argv.push_back("LASTSAVE");

    sendCommand(argv);

    return get_integer64();
}

bool redis_server::save()
{
    std::vector<std::string> argv;
    argv.push_back("SAVE");

    sendCommand(argv);
    return check_status();
}

/**
 * KillFilterParams ¿‡∂®“Â
 */
KillFilterParams& KillFilterParams::addr(const std::string & addr)
{
    params.push_back("ADDR");
    params.push_back(addr);
    return *this;
}

KillFilterParams& KillFilterParams::id(int id)
{
    params.push_back("ID");
    params.push_back(TO_STRING(id));
    return *this;
}

KillFilterParams& KillFilterParams::type(CLIENT_TYPE type)
{
    params.push_back("TYPE");
    params.push_back(TYPE_STR[type]);
    return *this;
}

KillFilterParams& KillFilterParams::skipme(bool yes /*= true*/)
{
    params.push_back("SKIPME");
    params.push_back(yes ? "yes" : "no");
    return *this;
}
