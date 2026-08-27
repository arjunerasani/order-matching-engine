// all includes will only be included once
#pragma once
#include <cstdint>

// this file represents an individual order in the order book

// here the enum is 8 bits to save memory
enum class orderType: uint8_t {
    BUY,
    SELL
};

struct order {
    // uint64_t is used to support large numbers of orders just like real life engines
    orderType orderType;
    uint64_t orderId;
    uint64_t ownerId;
    uint64_t price;
    uint32_t quantity;
    uint64_t timestamp;
};