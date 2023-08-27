#include"server_socket.hpp"
using std::string;

int main()
{
    // 1.create a socket
    // 2.bind to socket
    // 3.listen to the client
    ServerSocket server("127.0.0.1",8080);

    while (1)
    {
        // 4.receive the link from the client
        int connfd = server.accept();
        if (connfd < 0)
            return 1;

        Socket client(connfd);
        char buf[1024] = {0};

        // 5.receive the data from the client
        size_t len = client.recv(buf, sizeof(buf));
        printf("recv:: connfd =%d msg=%s\n", connfd, buf);

        // 6.send data to the client
        client.send(buf, len);
    }
    // 7.close the socket
    return 0;
}