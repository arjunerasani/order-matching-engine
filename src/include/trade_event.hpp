#pragma once
#include <cstdint>

struct tradeEvent {
    uint64_t tradeId;
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    uint64_t price;
    uint32_t quantity;
    uint64_t timestamp;
};