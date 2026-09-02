#pragma once
#include "order_book.hpp"
#include "order.hpp"
#include "price_level.hpp"
#include "matching_engine.hpp"

#include <iostream>
#include <cassert>
#include <chrono>
#include <vector>

class tests {
private:
    matchingEngine engine;

public:
    tests() = default;

    void testFullMatch();

    void testPartialFillAndQueue();

    void testMultiLevelSweep();

    void testTimePriorityExecution();
    
    void testNoCross();
};