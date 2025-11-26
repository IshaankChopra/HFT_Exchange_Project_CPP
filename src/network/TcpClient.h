#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <stdexcept>
#include <cstring>
#include <string>
#include <iostream>

class TcpClient {
private:
    int sockFd;
    struct sockaddr_in serverAddr;
    bool isConnected;

public:
    TcpClient() : sockFd(-1), serverAddr({}), isConnected(false) {};

    bool connectToExchange(const std::string& ip, int port) {
        sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockFd < 0) {
            throw std::runtime_error(std::string("TCP socket creation failed: ") + strerror(errno));
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
            perror("Invalid address/ Address not supported");
            return false;
        }

        if (connect(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("Connection Failed");
            return false;
        }

        int flag = 1;
        if (setsockopt(sockFd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)) < 0) {
            perror("Failed to set TCP_NODELAY");
        }

        isConnected = true;
        std::cout << "[Network] Connected to Exchange at " << ip << ":" << port << std::endl;
        return true;
    }

    void sendOrder(const void* data, const size_t size) {
        if (!isConnected) {
            std::cerr << "[Network] Error: Cannot send, not connected!" << std::endl;
            return;
        }

        ssize_t sent = send(sockFd, data, size, 0);
        if (sent < 0) {
            perror("Failed to send order");
        }
    }

    ~TcpClient() {
        if (sockFd >= 0) close(sockFd);
    }
};

#endif