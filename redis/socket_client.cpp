#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "socket_client.h"

int socket_client::connect_socket(const char * host, int port)
{
    int sockfd = 0;
    struct sockaddr_in addr;

    m_host = host;
    m_port = port;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    m_sockid = sockfd;

    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    addr.sin_addr.s_addr = inet_addr(host);
    if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        close_socket();
        return m_sockid;
    }
    else
    {
        m_sockid = sockfd;
        return m_sockid;
    }
}

int socket_client::close_socket()
{
    if (VALID_SOCKET(m_sockid)) {
        close(m_sockid);
        m_sockid = INVALID_SOCKET;
    }
    return 0;
}

bool socket_client::check_connect()
{
    if (!VALID_SOCKET(m_sockid))
    {
        if (connect_socket(m_host.c_str(), m_port) < 0)
        {
            return false;
        }
    }
    return true;
}

int socket_client::recv_msg(void * buff, int len)
{
    if (!check_connect())
    {
        return -1;
    }

    int read_len = 0;
//    while(read_len < len)
//    {
        int r_len = (int)recv(m_sockid, buff, (size_t)(len - read_len), 0);
        if(r_len > 0)
        {
            read_len += r_len;
        }
        else if(r_len == 0)
        {
            //break;  /* EOF */
        }
        else
        {
            close_socket();
            return -1;
        }
//    }

    return read_len;
}

int socket_client::send_msg(const char * buff)
{
    if (!check_connect())
    {
        return -1;
    }

    int sendSize = (int)send(m_sockid, buff, strlen(buff), 0);
    if (sendSize <= 0)
    {
        close_socket();
        return -1;
    }
    else
    {
        return sendSize;
    }
}

