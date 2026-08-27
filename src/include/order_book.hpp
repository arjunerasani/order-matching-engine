#pragma once
#include "price_level.hpp"
#include "order.hpp"

// map is a red black tree under the hood so it keeps the keys automatically sorted, which is what we want for the order book
#include <map>
#include <unordered_map>
#include <list>

struct orderLocation {
    orderType orderType;
    uint64_t price;

    // can't just have a normal iterator because the iterator needs to know what it is iterating over
    // this is just 1 value btw
    std::list<order>::iterator iterator; // this is a pointer to the node inside the list
};

// this file represents the order book, which is a collection of price levels for both buy and sell orders
// the bids are to be sorted in descending order, while the asks are to be sorted in ascending order
// using a class here because we want to encapsulate the data and operations for the order book, and we want to be able to create multiple instances of the order book if needed
class orderBook {
private:
    // in 8 byte integer is the data type for the key (price) and the priceLevel data type is for the value (priceLevel)
    std::map<uint64_t, priceLevel, std::greater<uint64_t>> bids; // descending order for bids
    std::map<uint64_t, priceLevel> asks; // ascending order for asks

    // the 8 byte integer here is for the key data type (orderId) and the orderLocation data type is for the value (orderLocation)
    // an unordered map is used because cancelling orders requires many "layers" to be explored leading to a large time complexity
    // this makes lookup O(1)
    std::unordered_map<uint64_t, orderLocation> orderIndex;

public:
    // changed this to a default constructor becuase the orderBook starts empty
    orderBook() = default;

    void addOrder(order& newOrder) {
        std::list<order>::iterator orderIterator;

        if (newOrder.orderType == orderType::BUY) {
            // if bids.find returns the end of the map then it doesn't exist because
            // bids.end() is the theoretical 1 space past the last element in the map
            if (bids.find(newOrder.price) == bids.end()) {
                priceLevel newPriceLevel = priceLevel{.price = newOrder.price, .orders = std::list<order>{newOrder}};

                // this means that a new key is created in bids (price) and a new value is assigned to the key (newPriceLevel)
                bids[newOrder.price] = newPriceLevel;

                // this gets the last order added and marks its location in the unordered map using an iterator
                // the iterator is grabbed from a reference directly to the map which doesn't get destroyed when this method ends
                // unlike newPriceLevel which is on the stack
                priceLevel& storedLevel = bids[newOrder.price];
                orderIterator = std::prev(storedLevel.orders.end());
            } else {
                // this finds the existing price level by using the key and then adds the new order
                // to the back of the price level
                // have to use a reference here so a copy isn't made
                priceLevel& existingPriceLevel = bids[newOrder.price];
                existingPriceLevel.appendOrderToBack(existingPriceLevel, newOrder);

                orderIterator = std::prev(existingPriceLevel.orders.end());
            }

            // this maps the orderId to the orderLocation which contains the iterator we got above
            orderIndex[newOrder.orderId] = orderLocation{orderType::BUY, newOrder.price, orderIterator};
        } else {
            if (asks.find(newOrder.price) == asks.end()) {
                priceLevel newPriceLevel = priceLevel{.price = newOrder.price, .orders = std::list<order>{newOrder}};
                asks[newOrder.price] = newPriceLevel;

                priceLevel& storedPriceLevel = asks[newOrder.price];
                orderIterator = std::prev(storedPriceLevel.orders.end());
            } else {
                priceLevel& existingPriceLevel = asks[newOrder.price];
                existingPriceLevel.appendOrderToBack(existingPriceLevel, newOrder);

                orderIterator = std::prev(existingPriceLevel.orders.end());
            }

            orderIndex[newOrder.orderId] = orderLocation{orderType::SELL, newOrder.price, orderIterator};
        }
    }

    void cancelOrder(uint64_t orderId) {
        // this returns a map pair with ->first holding the key and ->second holding the value
        auto mapIt = orderIndex.find(orderId);

        if (mapIt == orderIndex.end()) {
            std::cerr << "Error: Attempted to cancel an order that does not exist." << std::endl;
            return;
        } else {
            // use a reference here so it isn't a copy
            orderLocation& cancelledOrder = mapIt->second;

            if (cancelledOrder.orderType == orderType::BUY) {
                // have to erase the cancelled order from both the priceLevel.orders list its in and the unordered map
                // use a reference so the actual priceLevel is modified
                priceLevel& cancelledLevel = bids[cancelledOrder.price];

                // erase the cancelled order from the list of orders at this price level using the iterator stored in the orderLocation
                cancelledLevel.orders.erase(cancelledOrder.iterator);

                // this is for if the priceLevel is empty it has to be removed (frees memory)
                if (cancelledLevel.orders.empty()) {
                    bids.erase(cancelledOrder.price);
                }
            } else {
                priceLevel& cancelledLevel = asks[cancelledOrder.price];

                cancelledLevel.orders.erase(cancelledOrder.iterator);

                if (cancelledLevel.orders.empty()) {
                    asks.erase(cancelledOrder.price);
                }
            }

            // erase the cancelled order from the unordered map using the map pair iterator we got from the find function above
            orderIndex.erase(mapIt);
        }
    }

    // gets the best bid which is the first priceLevel (highest price) in the map
    // have to return a pointer because just returning normally with a return type that isn't void
    // will cause these methods to not compile
    priceLevel* getBestBid() {
        if (bids.begin() == bids.end()) {
            std::cerr << "Error: Attempted to get a priceLevel (bids) that doesn't exist." << std::endl;
            return nullptr;
        }

        return &bids.begin()->second;
    }

    // gets the best ask which is the first priceLevel (lowest price) in the map
    priceLevel* getBestAsk() {
        if (asks.begin() == asks.end()) {
            std::cerr << "Error: Attempted to get a priceLevel (asks) that doesn't exist." << std::endl;
            return nullptr;
        }

        return &asks.begin()->second;
    }
};
