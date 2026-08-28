#pragma once
#include "order.hpp"
#include <list>
#include <iostream>
#include <cstdint>

// this file represents all orders queued at a specific price level in the order book
// at a singular price level, orders should be executed in FIFO order, so a doubly linked list
// gives the best performance for this use case, as we can add and remove orders from the front and back of the list in O(1) time complexity

struct priceLevel {
    uint64_t price;
    std::list<order> orders;

    // operations for price level

    // appends an order to the back of the list of orders at this price level
    void appendOrderToBack(priceLevel& level, const order& newOrder) {
        level.orders.push_back(newOrder);
    }

    // removes an order from the front of the list of orders at this price level
    void removeOrderFromFront(priceLevel& level) {
        if (!level.orders.empty()) {
            level.orders.pop_front();
        } else {
            std::cerr << "Error: Attempted to remove an order from an empty price level." << std::endl;
        }
    }

    // removes an order by its orderId from the list of orders at this price level (for cancellations)
    void removeOrderById(priceLevel& level, uint64_t orderId) {
        // here i is an iterator that iterates through the list of orders at this price level
        for (auto i = level.orders.begin(); i != level.orders.end(); i++) {
            if (i->orderId == orderId) {
                level.orders.erase(i);
                break;
            }
        }
    }
};