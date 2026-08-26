#include "price_level.hpp"
// map is a red black tree under the hood so it keeps the keys automatically sorted, which is what we want for the order book
#include <map>

// this file represents the order book, which is a collection of price levels for both buy and sell orders
// the bids are to be sorted in descending order, while the asks are to be sorted in ascending order

struct orderBook {
    std::map<uint64_t, price_level, std::greater<uint64_t>> bids; // descending order for bids
    std::map<uint64_t, price_level> asks; // ascending order for asks
};

