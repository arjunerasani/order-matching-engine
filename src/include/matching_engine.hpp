#pragma once
#include "trade_event.hpp"
#include "order.hpp"

#include <list>

class matchingEngine {
private:
    orderBook orderBook;

public:
    matchingEngine() = default;
    
    std::list<tradeEvent> submitOrder(order incomingOrder);
};