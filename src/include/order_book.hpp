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

    void addOrder(order& newOrder);

    void cancelOrder(uint64_t orderId);

    priceLevel* getBestBid();

    priceLevel* getBestAsk();
};
