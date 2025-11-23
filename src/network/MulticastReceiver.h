#ifndef MULTICAST_RECEIVER_H
#define MULTICAST_RECEIVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>
#include <stdexcept>

class MulticastReceiver {
private:
    int sockFd;

public:
    MulticastReceiver(const std::string& mcastIp, const int port) {

        sockFd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockFd < 0) {
            throw std::runtime_error(std::string("Failed to create socket: ") + strerror(errno));
        }

        int reuse = 1;
        if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) < 0) {
            perror("Setting SO_REUSEADDR error");
            close(sockFd);
            exit(1);
        }

        struct sockaddr_in localAddr{};
        localAddr.sin_family = AF_INET;
        localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        localAddr.sin_port = htons(port);

        if (bind(sockFd, (struct sockaddr*)&localAddr, sizeof(localAddr)) < 0) {
            perror("Binding failed");
            close(sockFd);
            exit(1);
        }

        struct ip_mreq group;
        group.imr_multiaddr.s_addr = inet_addr(mcastIp.c_str());
        group.imr_interface.s_addr = htonl(INADDR_ANY);

        if (setsockopt(sockFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&group, sizeof(group)) < 0) {
            perror("Adding multicast group error");
            close(sockFd);
            exit(1);
        }

        std::cout << "[Network] Joined Multicast Group: " << mcastIp << " on Port " << port << std::endl;
    }

    ~MulticastReceiver() {
        if (sockFd >= 0) close(sockFd);
    }

    int receive(void* buffer, const size_t bufferSize) {
        struct sockaddr_in srcAddr;
        socklen_t len = sizeof(srcAddr);

        int nBytes = recvfrom(sockFd, buffer, bufferSize, 0, (struct sockaddr*)&srcAddr, &len);
        return nBytes;
    }
};

#endif