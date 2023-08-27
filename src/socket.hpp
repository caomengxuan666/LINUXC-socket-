#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

using std::string;

namespace yazi
{
    namespace socket
    {
        class Socket
        {
        public:
            Socket();
            virtual ~Socket();
            Socket(int sockfd);

            bool bind(const string &ip, int port);
            bool listen(int backlog);
            bool connect(const string &ip, int port);
            int accept();
            int send(const char *buf, size_t len);
            int recv(char *buf, size_t len);
            void close();

            bool set_non_blocking();
            bool set_send_buffer(int size);
            bool set_recv_buffer(int size);
            bool set_Linger(bool active, int seconds);
            bool set_keepalive();
            bool set_reuse_addr();

        protected:
            string m_ip;
            int m_port;
            int m_sockfd;
        };
    }
}

using namespace yazi::socket;
Socket::Socket()
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        printf("create socket error:error=%d\n", errno, strerror(errno));
    }
    printf("create socket success\n");
}

Socket::Socket(int sockfd) : m_ip(""), m_port(0), m_sockfd(sockfd) {}

Socket::~Socket()
{
    close();
}

bool Socket::bind(const string &ip, int port)
{
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;

    if (ip.empty())
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
    {
        sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
        sockaddr.sin_port = htons(port);
    }

    if (::bind(m_sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        printf("socket bind error:errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    m_ip = ip;
    m_port = port;
    printf("socket bind success: ip=%s port=%d\n", ip.c_str(), port);
    return true;
}

bool Socket::listen(int backlog)
{
    if (::listen(m_sockfd, 1024) < 0)
    {
        printf("socket listen error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    printf("socket listening\n");
    return true;
}

bool Socket::connect(const string &ip, int port)
{
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);
    if (::connect(m_sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        printf("socket connet error:errno=%d, errmsg=%s\n", errno, strerror(errno));
        return false;
    }
    m_ip = ip;
    m_port = port;
    printf("connet successfully\n");
    return true;
}

int Socket::accept()
{
    int connfd = ::accept(m_sockfd, NULL, NULL);
    if (connfd < 0)
    {
        printf("socket accept errno: errno=%d errmsg=%s\n", errno, strerror(errno));
        return -1;
    }
    printf("socket accept:conn=%d\n", connfd);
    return connfd;
}

int Socket::send(const char *buf, size_t len)
{
    return ::send(m_sockfd, buf, len, 0);
}

int Socket::recv(char *buf, size_t len)
{
    return ::recv(m_sockfd, buf, len, 0);
}

void Socket::close()
{
    if (m_sockfd > 0)
    {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
}

bool Socket::set_non_blocking()
{
    int flags = fcntl(m_sockfd, F_GETFL, 0);
    if (flags < 0)
    {
        printf("socket set_non_blocking error :errno=%d, errmsg=%s", errno, strerror(errno));
        return false;
    }
    flags |= O_NONBLOCK;
    if (fcntl(m_sockfd, F_SETFL, flags) < 0)
    {
        printf("socket set_non_blocking error: error=%d, errmsg=%s", errno, strerror(errno));
        return true;
    }
    return true;
}

bool Socket::set_send_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, &buff_size, sizeof(buff_size)) < 0)
    {
        printf("socket set_send_buffer error:errno=%d errmsg=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_recv_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, &buff_size, sizeof(buff_size)) < 0)
    {
        printf("socket set_send_buffer error:errno=%d errmsg=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_Linger(bool active, int seconds)
{
    struct linger l;
    memset(&l, 0, sizeof(l));
    l.l_onoff = active ? 1 : 0;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0)
    {
        printf("socket set_linger error: %d, errno=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_keepalive()
{
    int flag = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) < 0)
    {
        printf("socket set_keepalive error: errno=%d, errmsg=%s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_reuse_addr()
{
    int flag = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0)
    {
        printf("socket set_keepalive error: errno=%d, errmsg=%s", errno, strerror(errno));
        return false;
    }
    return true;
}