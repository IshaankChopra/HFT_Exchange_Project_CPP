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
    TcpClient* client; // Pointer to the connection used for execution
    std::atomic<int64_t> orderIdCounter;
    int currentPosition;
    const int MAX_POSITION_LIMIT = 5; // Max shares we can hold

public:
    // Dependency Injection: We pass the client in, rather than creating it inside
    StrategyEngine(const std::string& symbol, TcpClient* cli);
    // The Core Trigger: Called whenever a new packet arrives
    void onMarketData(const MarketUpdate& update);
};

#endif
