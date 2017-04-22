#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#include <string>

#define VALID_SOCKET(sock_id) ((sock_id) > 0)
#define INVALID_SOCKET -1
#define READ_CACHE_LEN 128

class socket_client
{
public:
    socket_client();
    ~socket_client() { close_socket(); }

public:
    int connect_socket(const char* host, int port);
    int close_socket();
    int recv_msg(void* buff, int len);
    int send_msg(const char* buff);
    int read_line(std::string& buff);

private:
    bool check_connect();
    int read_from_cache(void* buff, int len);

private:
    int m_sockid;
    std::string m_host;
    int m_port;

    char m_read_cache[READ_CACHE_LEN];
    int m_read_count;
    int m_read_idx;
};

#endif /* __SOCKET_CLIENT_H__ */
