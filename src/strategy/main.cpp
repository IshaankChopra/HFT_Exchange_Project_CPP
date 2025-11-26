#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include "../network/MulticastReceiver.h"
#include "../network/TcpClient.h"
#include "../common/Protocol.h"
#include "StrategyEngine.h"

std::atomic<bool> running(true);

TcpClient clientSCH;
TcpClient clientSCS;

void runMarketDataFeed(std::string symbol, std::string mcastIp, StrategyEngine* engine) {
    try {
        MulticastReceiver receiver(mcastIp, MCAST_PORT);
        MarketUpdate update;

        std::cout << "[Strategy] Listening for " << symbol << " on " << mcastIp << std::endl;

        while (running) {
            int bytes = receiver.receive(&update, sizeof(MarketUpdate));

            if (bytes == sizeof(MarketUpdate)) {
                engine->onMarketData(update);
            }
        }
    } catch (std::exception& e) {
        std::cerr << "[Error] Feed " << symbol << " crashed: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {

    std::cout << "=== HFT Strategy Runner Starting ===" << std::endl;

    while (!clientSCH.connectToExchange(EXCHANGE_IP, EXCHANGE_PORT_SCH)) {
        std::cout << "[Strategy] Connecting to SCH Exchange (" << EXCHANGE_PORT_SCH << ")..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "[Strategy] Connected to SCH Exchange." << std::endl;

    // Connect to SCS Exchange - Uncomment to run the 2nd exchange
    /*
    while (!clientSCS.connectToExchange(EXCHANGE_IP, EXCHANGE_PORT_SCS)) {
        std::cout << "[Strategy] Connecting to SCS Exchange (" << EXCHANGE_PORT_SCS << ")..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    */

    StrategyEngine engineSCH("SCH", &clientSCH);
    // StrategyEngine engineSCS ("SCS", &clientSCS);

    std::thread t1(runMarketDataFeed, "SCH", MCAST_GRP_SCH, &engineSCH);
    // std::thread t2(runMarketDataFeed, "SCS", MCAST_GRP_SCS, &engineSCS);

    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    t1.join();
    // t2.join();

    return 0;
}