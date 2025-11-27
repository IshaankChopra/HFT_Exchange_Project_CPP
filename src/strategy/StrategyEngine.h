#ifndef STRATEGY_ENGINE_H
#define STRATEGY_ENGINE_H

#include <string>
#include <atomic>
#include "OrderBook.h"
#include "../network/TcpClient.h"
#include "../common/Protocol.h"

class StrategyEngine {
private:
    std::string symbol;
    OrderBook orderBook;
    TcpClient* client;
    std::atomic<int64_t> orderIdCounter;
    int currentPosition;
    const int MAX_POSITION_LIMIT = 1000; // Max shares we can hold... not using as of now

public:
    // Dependency Injection
    StrategyEngine(const std::string& symbol, TcpClient* cli);
    void onMarketData(const MarketUpdate& update);
};

#endif
