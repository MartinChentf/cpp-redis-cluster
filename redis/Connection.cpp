#include "Connection.h"
#include "Protocol.h"
#include "socket_client.h"

Connection::Connection(const std::string & host, uint16_t port)
: m_host(host)
, m_port(port)
, m_socket(NULL)
{}

Connection::~Connection()
{
    if (m_socket) {
        m_socket->close_socket();
        delete m_socket;
        m_socket = NULL;
    }
}

void Connection::connect()
{
    if (m_socket == NULL) {
        m_socket = new socket_client(m_host, m_port);
        m_socket->connect_socket();
    }
}

bool Connection::sendCommand(const std::vector<std::string>& argv)
{
    connect();

    return Protocol::sendCommand(m_socket, argv);
}

redisReply* Connection::receiveReply()
{
    return Protocol::process(m_socket);
}
