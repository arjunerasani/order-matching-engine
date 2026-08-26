#include "price_level.hpp"
#include "order.hpp"

// map is a red black tree under the hood so it keeps the keys automatically sorted, which is what we want for the order book
#include <map>
#include <list>

// this file represents the order book, which is a collection of price levels for both buy and sell orders
// the bids are to be sorted in descending order, while the asks are to be sorted in ascending order
// using a class here because we want to encapsulate the data and operations for the order book, and we want to be able to create multiple instances of the order book if needed
class orderBook {
private:
    std::map<uint64_t, priceLevel, std::greater<uint64_t>> bids; // descending order for bids
    std::map<uint64_t, priceLevel> asks; // ascending order for asks

public:
    orderBook(std::map<uint64_t, priceLevel, std::greater<uint64_t>> b, std::map<uint64_t, priceLevel> a) {
        bids = b;
        asks = a;
    }

    void addOrder(order& newOrder) {
        if (newOrder.orderType == orderType::BUY) {
            // if bids.find returns the end of the map then it doesn't exist because
            // bids.end() is the theoretical 1 space past the last element in the map
            if (bids.find(newOrder.price) == bids.end()) {
                priceLevel newPriceLevel = priceLevel{.price = newOrder.price, .orders = std::list<order>{newOrder}};

                // this means that a new key is created in bids (price) and a new value is assigned to the key (newPriceLevel)
                bids[newOrder.price] = newPriceLevel;
            } else {
                // this finds the existing price level by using the key and then adds the new order
                // to the back of the price level
                priceLevel existingPriceLevel = bids[newOrder.price];
                existingPriceLevel.appendOrderToBack(existingPriceLevel, newOrder);
            }
        } else {
            if (asks.find(newOrder.price) == asks.end()) {
                priceLevel newPriceLevel = priceLevel{.price = newOrder.price, .orders = std::list<order>{newOrder}};
                asks[newOrder.price] = newPriceLevel;
            } else {
                priceLevel existingPriceLevel = asks[newOrder.price];
                existingPriceLevel.appendOrderToBack(existingPriceLevel, newOrder);
            }
        }
    }

    void cancelOrder(uint64_t orderId) {
        
    }
};
