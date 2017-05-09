#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#include <string>

#define VALID_SOCKET(sock_id) ((sock_id) > 0)
#define INVALID_SOCKET -1
#define READ_BUFF_LEN 8192
#define WRITE_BUFF_LEN 8192

class socket_client
{
public:
    socket_client();
    ~socket_client() { close_socket(); }

public:
    int connect_socket(const char* host, int port);
    int close_socket();

    char read_byte();
    std::string read_line();
    int read(void* buff, int len);

    int write(const void* buff, int len);
    void flush();

private:
    bool check_connect();

    /**
     * 确保缓冲区有可读数据, 返回缓冲区可读字节长度.
     */
    int read_to_buff();
    int flush_buffer();

private:
    int m_sockid;
    std::string m_host;
    int m_port;

    char m_read_buff[READ_BUFF_LEN];
    int m_read_count;
    int m_read_limit;

    char m_write_buff[WRITE_BUFF_LEN];
    int m_write_count;
};

#endif /* __SOCKET_CLIENT_H__ */
