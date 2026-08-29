#pragma once
#include "trade_event.hpp"
#include "order.hpp"

#include <list>

class matchingEngine {
private:
    orderBook book;
    uint64_t ids;

public:
    matchingEngine() = default;
    
    std::list<tradeEvent> submitOrder(order incomingOrder);
};