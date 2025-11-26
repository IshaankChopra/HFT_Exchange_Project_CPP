#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>

#define MCAST_GRP_SCH "239.0.0.1"
#define MCAST_GRP_SCS "239.0.0.2"
#define MCAST_PORT 30001
#define EXCHANGE_IP "127.0.0.1"
#define EXCHANGE_PORT_SCH 8080
#define EXCHANGE_PORT_SCS 8081;

#pragma pack(push, 1)

struct MarketUpdate {
    int64_t epoch;
    int64_t orderId;
    double price;
    int32_t quantity;
    char symbol[8];
    char side;
    char type;
};

struct OrderRequest {
    int64_t orderId;
    double price;
    int32_t quantity;
    char symbol[8];
    char side;
};

struct ExecutionReport {
    int64_t orderId;
    int32_t status;
    double fillPrice;
    int32_t fillQty;
};

#pragma pack(pop)

#endif
