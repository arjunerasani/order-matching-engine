#include "include/order_book.hpp"
#include "include/order.hpp"
#include "include/price_level.hpp"

#include <iostream>
#include <cassert>
#include <chrono>

void testAddAndGetBestBidAsk() {
    orderBook book;

    assert(book.getBestBid() == nullptr);
    assert(book.getBestAsk() == nullptr);

    order buy1{orderType::BUY, 1, 101, 100, 10, 1000};
    order buy2{orderType::BUY, 2, 102, 105, 15, 1001};

    book.addOrder(buy1);
    book.addOrder(buy2);

    assert(book.getBestBid() != nullptr);
    assert(book.getBestBid()->price == 105);

    order sell1{orderType::SELL, 3, 103, 110, 5, 1002};
    order sell2{orderType::SELL, 4, 104, 115, 8, 1003};

    book.addOrder(sell1);
    book.addOrder(sell2);

    assert(book.getBestAsk() != nullptr);
    assert(book.getBestAsk()->price == 110);
}

void testTimePriorityAtSamePrice() {
    orderBook book;

    order buy1{orderType::BUY, 1, 101, 100, 10, 1000};
    order buy2{orderType::BUY, 2, 102, 100, 20, 1001};

    book.addOrder(buy1);
    book.addOrder(buy2);

    priceLevel* bestBid = book.getBestBid();
    assert(bestBid != nullptr);
    assert(bestBid->orders.front().orderId == 1);
}

void testCancelOrderAndPriceLevelCleanup() {
    orderBook book;

    order buy1{orderType::BUY, 1, 101, 100, 10, 1000};
    order buy2{orderType::BUY, 2, 102, 100, 15, 1001};

    book.addOrder(buy1);
    book.addOrder(buy2);

    book.cancelOrder(1);
    assert(book.getBestBid()->orders.front().orderId == 2);

    book.cancelOrder(2);
    assert(book.getBestBid() == nullptr);

    book.cancelOrder(999);
}

// benchmark function: runs thousands of insertions and cancellations
void testBulkPerformance(int numOrders) {
    orderBook book;

    auto start = std::chrono::high_resolution_clock::now();

    // 1. Bulk insertion test
    for (int i = 1; i <= numOrders; ++i) {
        order buy{orderType::BUY, static_cast<uint64_t>(i), 100, static_cast<uint64_t>(1000 + (i % 50)), 10, 1000};
        book.addOrder(buy);
    }

    auto inserted = std::chrono::high_resolution_clock::now();

    // 2. Bulk cancellation test
    for (int i = 1; i <= numOrders; ++i) {
        book.cancelOrder(i);
    }

    auto finished = std::chrono::high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(inserted - start).count();
    auto cancelTime = std::chrono::duration_cast<std::chrono::microseconds>(finished - inserted).count();

    std::cout << "\n--- Step 1 Bulk Micro-Benchmark (" << numOrders << " operations) ---" << std::endl;
    std::cout << "Insertions: " << insertTime << " us (" << (double)numOrders / insertTime * 1000.0 << " ops/ms)" << std::endl;
    std::cout << "Cancellations: " << cancelTime << " us (" << (double)numOrders / cancelTime * 1000.0 << " ops/ms)" << std::endl;
}

int main() {
    std::cout << "--- Running OrderBook Step 1 Unit Tests ---" << std::endl;

    auto startAll = std::chrono::high_resolution_clock::now();

    testAddAndGetBestBidAsk();
    std::cout << "[PASS] Add Order & Best Bid/Ask Priority Test" << std::endl;

    testTimePriorityAtSamePrice();
    std::cout << "[PASS] Time Priority Test" << std::endl;

    testCancelOrderAndPriceLevelCleanup();
    std::cout << "[PASS] Cancel Order & Empty Level Cleanup Test" << std::endl;

    auto endAll = std::chrono::high_resolution_clock::now();
    auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(endAll - startAll).count();

    std::cout << "\nUnit Tests Execution Time: " << totalDuration << " us (" << totalDuration / 1000.0 << " ms)" << std::endl;

    // Run micro-benchmark with 100,000 orders
    testBulkPerformance(100000);

    return 0;
}