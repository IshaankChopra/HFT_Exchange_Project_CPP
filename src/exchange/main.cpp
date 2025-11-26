#include <iostream>
#include <thread>
#include <atomic>
#include <cstring>
#include <unistd.h>
#include "../network/TcpServer.h"
#include "../common/Protocol.h"

void handleClient(int clientSocket) {
    OrderRequest order;

    while (true) {

        ssize_t bytesRead = recv(clientSocket, &order, sizeof(OrderRequest), 0);

        if (bytesRead <= 0) {
            std::cout << "[Exchange] Client disconnected." << std::endl;
            close(clientSocket);
            break;
        }

        if (bytesRead != sizeof(OrderRequest)) {
            std::cerr << "[Strategy] Error: Received incomplete packet." << std::endl;
            continue;
        }

        std::cout << "[Exchange] RECV: "
                  << (order.side == 'B' ? "BUY " : "SELL ")
                  << order.quantity << " " << order.symbol
                  << " @ " << order.price
                  << " (ID: " << order.orderId << ")" << std::endl;

        ExecutionReport report;
        report.orderId = order.orderId;
        report.status = 1;
        report.fillPrice = order.price;
        report.fillQty = order.quantity;

        send(clientSocket, &report, sizeof(ExecutionReport), 0);
        std::cout << "[Exchange] SENT: Fill Confirmation" << std::endl;
    }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);

    try {
        TcpServer server(port);

        while (true) {
            std::cout << "[Exchange] Waiting for traders..." << std::endl;

            int clientSock = server.acceptConnection();

            if (clientSock >= 0) {
                // Spawn a thread to handle this client specifically
                // This allows multiple strategies to trade at once
                std::thread t(handleClient, clientSock);
                t.detach(); // Let it run in the background
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Server crash: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}