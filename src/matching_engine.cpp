#include "matching_engine.hpp"
#include "order_book.hpp"

#include <chrono>

std::list<tradeEvent> matchingEngine::submitOrder(order incomingOrder) {
    std::list<tradeEvent> allTrades = {};

    // this is the first check to see if the quantity of buying/selling is greater than zero
    while (incomingOrder.quantity > 0) {
        if (incomingOrder.orderType == orderType::BUY) {
            // second check to see if the other order book based on your order type is empty
            if (book.getBestAsk() == nullptr) {
                break;
            }

            // this is to check if a cross exists ie the buy order proposes a high enough price for the seller
            if (incomingOrder.price < book.getBestAsk()->price) {
                break;
            }

            // this gets the order that has been there in the price level the longest (FIFO)
            order& restingOrder = book.getBestAsk()->orders.front();

            // this gets the minimum quantity that this buy order can get from this sell
            auto tradeQuantity = std::min(incomingOrder.quantity, restingOrder.quantity);

            // the trade should always go through at the price of the resting order
            // this is because of price-time priority, the order that has been there the longest
            // (restingOrder) has to be the price taken
            auto tradePrice = restingOrder.price;

            // subtract the trade quantities from the order's quantities for later
            incomingOrder.quantity -= tradeQuantity;
            restingOrder.quantity -= tradeQuantity;

            // this gets the current time
            uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            // create a new trade event
            tradeEvent newTradeEvent = {ids, incomingOrder.orderId, restingOrder.orderId, tradePrice, tradeQuantity, timestamp};

            // increase the id count
            ids++;

            // finally add the trade to the list
            allTrades.push_back(newTradeEvent);

            // this removes resting order if its quantity is 0 and also erases it from the orderIndex
            // also erases the price level if it is empty
            if (restingOrder.quantity == 0) {
                book.cancelOrder(restingOrder.orderId);
            }
        } else {
            if (book.getBestBid() == nullptr) {
                break;
            }

            // this is to check if a cross exists ie the sell order proposes a low enough price for the buyer
            if (incomingOrder.price > book.getBestBid()->price) {
                break;
            }

            order& restingOrder = book.getBestBid()->orders.front();

            auto tradeQuantity = std::min(incomingOrder.quantity, restingOrder.quantity);

            auto tradePrice = restingOrder.price;

            incomingOrder.quantity -= tradeQuantity;
            restingOrder.quantity -= tradeQuantity;

            uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            tradeEvent newTradeEvent = {ids, restingOrder.orderId, incomingOrder.orderId, tradePrice, tradeQuantity, timestamp};

            ids++;

            allTrades.push_back(newTradeEvent);

            if (restingOrder.quantity == 0) {
                book.cancelOrder(restingOrder.orderId);
            }
        }
    }

    // this checks to see if the incomingOrder has a quantity of 0 and if it doesn't
    // then adds it to the orderBook as a resting order
    // this is done outside because we want the order to look at as many resting orders as possible to try and
    // fill it
    if (incomingOrder.quantity > 0) {
        book.addOrder(incomingOrder);
    }

    return allTrades;
}