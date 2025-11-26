#ifndef MULTICAST_SENDER_H
#define MULTICAST_SENDER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>
#include <stdexcept>

class MulticastSender {
private:
    int sockFd;
    struct sockaddr_in addr{};

public:
    MulticastSender(const std::string& ip, const int port) {
        sockFd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockFd < 0) {
            throw std::runtime_error(std::string("Failed to create socket: ") + strerror(errno));
        }

        addr.sin_family = AF_INET;
        if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
            close(sockFd);
            throw std::invalid_argument("Invalid IP address: " + ip);
        }
        addr.sin_port = htons(port);

        int loop = 1;
        if (setsockopt(sockFd, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&loop, sizeof(loop)) < 0) {
            perror("Setting IP_MULTICAST_LOOP error");
            close(sockFd);
            exit(1);
        }

        std::cout << "[Network] sender ready on " << ip << ":" << port << std::endl;
    }

    ~MulticastSender() {
        if (sockFd >= 0) close(sockFd);
    }

    void send (const void *data, const size_t size) {
        ssize_t sentBytes = sendto(sockFd, data, size, 0, (struct sockaddr*)&addr, sizeof(addr));
        if (sentBytes < 0) {
            perror("Send failed");
        }
    }


};

#endif