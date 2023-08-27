#pragma once
#include "socket.hpp"
namespace yazi
{
    namespace socket
    {
        class ClientSocket : public Socket
        {
        public:
            ClientSocket() = delete;
            ClientSocket(const string &ip, int port);
            ~ClientSocket() = default;
        };
    }
}

ClientSocket::ClientSocket(const string &ip, int port) : Socket()
{
    connect(ip, port);
}