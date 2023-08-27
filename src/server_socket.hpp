#pragma once
#include "socket.hpp"

#define msgsize 10

namespace yazi
{
    namespace socket
    {
        class ServerSocket : public Socket
        {
        public:
            ServerSocket() = delete;
            ServerSocket(const string &ip, int port);
            ~ServerSocket() = default;
        };
    }
}

ServerSocket::ServerSocket(const string &ip, int port):Socket()
{
    // set_non_blocking();
    set_recv_buffer(msgsize * 1024);
    set_send_buffer(msgsize * 1024);
    set_Linger(true,0);
    set_keepalive();
    set_reuse_addr();
    bind(ip,port);
    listen(1024);
}