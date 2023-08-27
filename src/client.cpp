#include "client_socket.hpp"

using std::string;

int main()
{
    // 1.create a socket
    // 2.link to the server
    ClientSocket client("127.0.0.1", 8080);

    // 3.send data to the server
    const string data = "hello cmx";
    client.send(data.c_str(), data.size());

    // 4.recieve the data from ther server
    char buf[1024] = {0};
    client.recv(buf, sizeof(buf));
    printf("recv:: msg=%s\n", buf);

    // 5.close the socket
    return 0;
}