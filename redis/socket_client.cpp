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
, m_read_idx(0)
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

int socket_client::recv_msg(void * buff, int len)
{
    if (!check_connect())
    {
        return -1;
    }

    int read_len = 0;
    read_len = (int)recv(m_sockid, buff, (size_t)len, 0);
    if(read_len < 0) {
        close_socket();
        return -1;
    }
    else if(read_len == 0) {
        return 0;
    }

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

int socket_client::read_line(std::string& buff)
{
	char ch;

    while (true) {
        if (m_read_idx >= m_read_count) {
            m_read_count = recv_msg(m_read_cache, READ_CACHE_LEN);
            m_read_idx = 0;
            if (m_read_count <= 0) break;
        }

        ch = m_read_cache[m_read_idx++];
        buff.append(&ch);

        if (ch == '\n') {
            break;
        }
    }

	return buff.size();
}

