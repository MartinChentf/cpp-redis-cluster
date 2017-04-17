#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#include <string>

#define VALID_SOCKET(sock_id) ((sock_id) > 0)
#define INVALID_SOCKET -1

class socket_client
{
public:
    socket_client():m_sockid(INVALID_SOCKET), m_host(""), m_port(0){}
    virtual ~socket_client() { close_socket(); }

public:
    int connect_socket(const char* host, int port);
    int close_socket();
    int recv_msg(void* buff, int len);
    int send_msg(const char* buff);

private:
    bool check_connect();

private:
    int m_sockid;
    std::string m_host;
    int m_port;
};

#endif /* __SOCKET_CLIENT_H__ */
