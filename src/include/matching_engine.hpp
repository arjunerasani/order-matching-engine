#pragma once
#include "trade_event.hpp"
#include "order.hpp"
#include "order_book.hpp"

#include <list>

class matchingEngine {
private:
    orderBook book;
    uint64_t ids;

public:
    matchingEngine() = default;
    
    std::list<tradeEvent> submitOrder(order incomingOrder);

    // expose the orderBook's methods for adding and canceling orders, as well as getting the best bid and ask
    // this allows for testing to access everything through the engine itself instead of the orderbook directly, which is more realistic to how a real matching engine would be used
    void addOrder(order& newOrder) {
        book.addOrder(newOrder);
    }

    void cancelOrder(uint64_t orderId) {
        book.cancelOrder(orderId);
    }

    priceLevel* getBestBid() {
        return book.getBestBid();
    }

    priceLevel* getBestAsk() {
        return book.getBestAsk();
    }
};