#include <iostream>
#include <thread>
#include <chrono>
#include "FileParser.h"
#include "../network/MulticastSender.h"
#include "../common/Protocol.h"

int main(int argc, char* argv[]) {

    // Requirement: "Two market data injectors... runs as a separate process"
    // Usage: ./market_injector data/SCH_test.log 239.0.0.1

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <logfile> <multicast_ip>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string mcastIp = argv[2];

    std::cout << "Starting injector for " << filename << " on " << mcastIp << std::endl;

    FileParser parser(filename);
    MulticastSender sender(mcastIp, MCAST_PORT);

    MarketUpdate update;
    long long messageCount = 0;

    while (parser.parseNext(update)) {
        sender.send(&update, sizeof(MarketUpdate));
        messageCount++;
        // TODO: Throttle speed?
        // In real life, we sleep to match the timestamp diffs.
        // For this test, we blast as fast as possible to stress test the Strategy.
        // If Strategy crashes, uncomment the line below:
        // std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    std::cout << "Finished. Sent " << messageCount << " messages." << std::endl;
    return 0;
}
