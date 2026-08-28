#include "matching_engine.hpp"
#include "order_book.hpp"

std::list<tradeEvent> matchingEngine::submitOrder(order incomingOrder) {
    // this is the first check to see if the quantity of buying/selling is greater than zero
    while (incomingOrder.quantity > 0) {
        if (incomingOrder.orderType == orderType::BUY) {
            // second check to see if the other order book based on your order type is empty
            if (orderBook.getBestAsk() == nullptr) {
                std::cerr << "Error: Attempted to get a matching order from empty asks." << std::endl;
                break;
            }

            // this is to check if a cross exists ie the buy order proposes a high enough price for the seller
            if (incomingOrder.price < orderBook.getBestAsk()->price) {
                break;
            }

            // this gets the order that has been there in the price level the longest (FIFO)
            order restingOrder = orderBook.getBestAsk()->orders.front();

            // this gets the minimum quantity that this buy order can get from this sell
            auto tradeQuantity = std::min(incomingOrder.quantity, restingOrder.quantity);

            // the trade should always go through at the price of the resting order
            // this is because of price-time priority, the order that has been there the longest
            // (restingOrder) has to be the price taken
            auto tradePrice = restingOrder.price;
        } else {
            if (orderBook.getBestBid() == nullptr) {
                std::cerr << "Error: Attempted to get a matching order from empty bids." << std::endl;
                break;
            }

            // this is to check if a cross exists ie the sell order proposes a low enough price for the buyer
            if (incomingOrder.price > orderBook.getBestBid()->price) {
                break;
            }

            order restingOrder = orderBook.getBestBid()->orders.front();

            auto tradeQuantity = std::min(incomingOrder.quantity, restingOrder.quantity);

            auto tradePrice = restingOrder.price;
        }
    }
}