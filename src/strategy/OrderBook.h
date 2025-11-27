#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <map>
#include <iostream>
#include "../common/Protocol.h"

class OrderBook {
private:
    std::map<double, int32_t, std::greater<double>> bids;
    std::map<double, int32_t> asks;
    struct OrderEntry {
        double price;
        int32_t quantity;
        char side;
    };
    std::map<int64_t, OrderEntry> orderMap;

public:
    void processUpdate(const MarketUpdate& update) {
        if (update.type == 'N') {
            handleNew(update);
        }
        else if (update.type == 'C') {
            handleCancel(update);
        }
        else if (update.type == 'T') {
            handleTrade(update);
        }
    }

    void consumeLiquidity(double price, int32_t quantity, char side) {
        if (side == 'B') {
            if (asks.find(price) != asks.end()) {
                asks[price] -= quantity;
                if (asks[price] <= 0) asks.erase(price);
            }
        }else {
            if (bids.find(price) != bids.end()) {
                bids[price] -= quantity;
                if (bids[price] <= 0) bids.erase(price);
            }
        }
    }

    double getBestBid() const {
        if (bids.empty()) return -1.0;
        return bids.begin()->first;
    }

    double getBestAsk() const {
        if (asks.empty()) return -1.0;
        return asks.begin()->first;
    }
private:
    void handleNew(const MarketUpdate& u) {
        orderMap[u.orderId] = {u.price, u.quantity, u.side};
        if (u.side == 'B') {
            bids[u.price] += u.quantity;
        } else {
            asks[u.price] += u.quantity;
        }
    }

    void handleCancel(const MarketUpdate &u) {
        auto it = orderMap.find(u.orderId);
        if (it == orderMap.end()) return;

        OrderEntry& ord = it->second;
        if (ord.side == 'B') {
            bids[ord.price] -= ord.quantity;
            if (bids[ord.price] <= 0) bids.erase(ord.price);
        } else {
            asks[ord.price] -= ord.quantity;
            if (asks[ord.price] <= 0) asks.erase(ord.price);
        }
        orderMap.erase(it);
    }

    void handleTrade (const MarketUpdate &u) {
        if (u.side == 'B') {
            bids[u.price] -= u.quantity;
            if (bids[u.price] <= 0) bids.erase(u.price);
        } else {
            asks[u.price] -= u.quantity;
            if (asks[u.price] <= 0) asks.erase(u.price);
        }
    }

    void printBestPrices() {
        std::cout << "Book | Best Bid: " << getBestBid()
                  << " | Best Ask: " << getBestAsk() << std::endl;
    }
};

#endif
