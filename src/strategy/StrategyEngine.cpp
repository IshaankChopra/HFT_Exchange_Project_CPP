#include "StrategyEngine.h"
#include "../common/Protocol.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

StrategyEngine::StrategyEngine(const std::string &sym, TcpClient *cli)
    : symbol(sym), client(cli), orderIdCounter(1), currentPosition(0) {}

void StrategyEngine::onMarketData(const MarketUpdate &update) {

    orderBook.processUpdate(update);

    double bestAsk = orderBook.getBestAsk();
    if (bestAsk > 0 && bestAsk < 108.00) {

        if (currentPosition >= MAX_POSITION_LIMIT) {
            return;
        }

        OrderRequest order;
        // Generate a UNIQUE ID for this order
        // fetch_add(1) increments the counter and returns the previous value
        // This is atomic, so two threads can't have the same ID
        order.orderId = orderIdCounter.fetch_add(1);
        order.price = bestAsk;
        order.quantity = 1;

        memset(order.symbol, 0, 8);
        strncpy(order.symbol, symbol.c_str(), 7);
        order.side = 'B';

        if (client) {
            client->sendOrder(&order, sizeof(OrderRequest));
            currentPosition += order.quantity;
            orderBook.consumeLiquidity(bestAsk, order.quantity, 'B');
            std::cout << " >>> [" << symbol << "] STRATEGY SIGNAL: BUY @ " << bestAsk
                      << " (Order ID: " << order.orderId << ")" << std::endl;
        }
    }
}

