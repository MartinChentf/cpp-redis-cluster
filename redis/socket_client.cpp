#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "socket_client.h"

socket_client::socket_client()
: m_sockid(INVALID_SOCKET)
, m_host("")
, m_port(0)
, m_read_count(0)
, m_read_limit(0)
{
    memset(m_read_cache, 0, READ_CACHE_LEN);
}

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

int socket_client::read_to_cache()
{
    if (m_read_count >= m_read_limit) {
        if (!check_connect()) {
            return -1;
        }
        m_read_limit =
            (int)recv(this->m_socket, m_read_cache, READ_CACHE_LEN, 0);
        m_read_count = 0;

        if(m_read_limit < 0) {
            close_socket();
            return -1;
        }

        return m_read_limit;
    }
    return m_read_limit - m_read_count;
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

char socket_client::read_byte()
{
    read_to_cache();
    return m_read_cache[m_read_count++];
}

std::string socket_client::read_line()
{
    std::string reply;
    while (true) {
        read_to_cache();

        char b = m_read_cache[m_read_count++];
        if (b == '\r') {
            read_to_cache();

            char c = m_read_cache[m_read_count++];
            if (c == '\n') {
                break;
            }
            reply.push_back(b);
            reply.push_back(c);
        }
        else {
            reply.push_back(b);
        }
    }

	return reply;
}

int socket_client::read(void* buff, int len)
{
    int offset = 0;
    while (offset < len) {
        read_to_cache();

        int avail_len = m_read_limit - m_read_count;
        int request_len = len - offset;
        int length = (avail_len < request_len) ? avail_len : request_len;

        memcpy(buff + offset, m_read_cache + m_read_count, length);
        m_read_count += length;
        offset += length;
    }

    return len;
}

int socket_client::write(const void* buff, int len)
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

