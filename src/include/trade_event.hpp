#pragma once
#include <cstdint>

// everything needed for a tradeEvent to happen
enum status: uint8_t {
    FILLED,
    PARTIALLY_FILLED,
    QUEUED
};

struct tradeEvent {
    status tradeStatus;
    uint64_t tradeId;
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    uint64_t buyerId;
    uint64_t sellerId;
    uint64_t price;
    uint32_t quantity;
    uint64_t timestamp;
};