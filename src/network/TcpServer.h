#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cstring>

class TcpServer {
    int serverFd;
    int port;

public:
    TcpServer(int port): serverFd(-1), port(port) {

        serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverFd < 0) {
           throw std::runtime_error(std::string("TCP Server socket failed: ") + strerror(errno));
        }

        int opt = 1;
        if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
            perror("Failed to set SO_REUSEADDR");
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("Bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(serverFd, 3) < 0) {
            perror("Listen");
            exit(EXIT_FAILURE);
        }

        std::cout << "[Network] TCP Server listening on port " << port << std::endl;
    }

    int acceptConnection() {
        struct sockaddr_in addr;
        int addrlen = sizeof(addr);

        int newSocket = accept(serverFd, (struct sockaddr*)&addr, (socklen_t*)&addrlen);
        if (newSocket < 0) {
            perror("Accept");
            return -1;
        }
        std::cout << "[Network] Accepted connection from client." << std::endl;
        return newSocket;
    }

    ~TcpServer() {
        if (serverFd >= 0) close(serverFd);
    }
};

#endif