#include "include/order_book.hpp"
#include "include/order.hpp"
#include "include/price_level.hpp"
#include "include/matching_engine.hpp"

#include <iostream>
#include <cassert>
#include <chrono>
#include <vector>

// -----------------------------------------------------------------------------
// Test 1: Full Match (Exact Fill)
// -----------------------------------------------------------------------------
void testFullMatch() {
    matchingEngine engine;

    // Resting Ask: Sell 10 @ $100
    order sell1{orderType::SELL, 1, 101, 100, 10, 1000};
    engine.submitOrder(sell1);

    // Incoming Buy: Buy 10 @ $100
    order buy1{orderType::BUY, 2, 102, 100, 10, 1001};
    auto trades = engine.submitOrder(buy1);

    // Verifications
    assert(trades.size() == 1);
    auto trade = trades.front();
    assert(trade.buyOrderId == 2);
    assert(trade.sellOrderId == 1);
    assert(trade.price == 100);
    assert(trade.quantity == 10);

    // Book should now be empty on both sides
    assert(engine.getBestAsk() == nullptr);
    assert(engine.getBestBid() == nullptr);
}

// -----------------------------------------------------------------------------
// Test 2: Partial Fill & Queue Remainder
// -----------------------------------------------------------------------------
void testPartialFillAndQueue() {
    matchingEngine engine;

    // Resting Ask: Sell 5 @ $100
    order sell1{orderType::SELL, 1, 101, 100, 5, 1000};
    engine.submitOrder(sell1);

    // Incoming Buy: Buy 15 @ $100 (Fills 5, 10 remaining)
    order buy1{orderType::BUY, 2, 102, 100, 15, 1001};
    auto trades = engine.submitOrder(buy1);

    assert(trades.size() == 1);
    assert(trades.front().quantity == 5);

    // Ask level should be completely drained
    assert(engine.getBestAsk() == nullptr);

    // Remaining 10 buy quantity should rest as Best Bid @ $100
    const priceLevel* bestBid = engine.getBestBid();
    assert(bestBid != nullptr);
    assert(bestBid->price == 100);
    assert(bestBid->orders.front().quantity == 10);
    assert(bestBid->orders.front().orderId == 2);
}

// -----------------------------------------------------------------------------
// Test 3: Multi-Level Price Sweep
// -----------------------------------------------------------------------------
void testMultiLevelSweep() {
    matchingEngine engine;

    // Set up ask ladder:
    // Level $100: 5 shares
    // Level $101: 10 shares
    // Level $102: 15 shares
    engine.submitOrder(order{orderType::SELL, 1, 101, 100, 5, 1000});
    engine.submitOrder(order{orderType::SELL, 2, 102, 101, 10, 1001});
    engine.submitOrder(order{orderType::SELL, 3, 103, 102, 15, 1002});

    // Incoming aggressive buy: Buy 20 @ $102
    // Expected fills: 5 @ $100, 10 @ $101, 5 @ $102 (leaving 10 @ $102 resting)
    order aggressiveBuy{orderType::BUY, 4, 104, 102, 20, 1003};
    auto trades = engine.submitOrder(aggressiveBuy);

    assert(trades.size() == 3);

    auto it = trades.begin();
    assert(it->price == 100 && it->quantity == 5);  it++;
    assert(it->price == 101 && it->quantity == 10); it++;
    assert(it->price == 102 && it->quantity == 5);

    // Best Ask should now be $102 with 10 shares remaining
    const priceLevel* bestAsk = engine.getBestAsk();
    assert(bestAsk != nullptr);
    assert(bestAsk->price == 102);
    assert(bestAsk->orders.front().quantity == 10);
}

// -----------------------------------------------------------------------------
// Test 4: Time Priority at Same Price Level (FIFO)
// -----------------------------------------------------------------------------
void testTimePriorityExecution() {
    matchingEngine engine;

    // Two sell orders at the exact same price
    order sellEarliest{orderType::SELL, 1, 101, 100, 10, 1000};
    order sellLater{orderType::SELL, 2, 102, 100, 10, 1001};

    engine.submitOrder(sellEarliest);
    engine.submitOrder(sellLater);

    // Incoming buy for 10 shares should match exclusively with sellEarliest (ID 1)
    order buy{orderType::BUY, 3, 103, 100, 10, 1002};
    auto trades = engine.submitOrder(buy);

    assert(trades.size() == 1);
    assert(trades.front().sellOrderId == 1);

    // The remaining ask should be sellLater (ID 2)
    const priceLevel* bestAsk = engine.getBestAsk();
    assert(bestAsk != nullptr);
    assert(bestAsk->orders.front().orderId == 2);
}

// -----------------------------------------------------------------------------
// Test 5: No Cross (Order Book Queueing)
// -----------------------------------------------------------------------------
void testNoCross() {
    matchingEngine engine;

    // Bid @ $99, Ask @ $101 (Spread = $2, no cross)
    order buy{orderType::BUY, 1, 101, 99, 10, 1000};
    order sell{orderType::SELL, 2, 102, 101, 10, 1001};

    auto buyTrades = engine.submitOrder(buy);
    auto sellTrades = engine.submitOrder(sell);

    assert(buyTrades.empty());
    assert(sellTrades.empty());

    assert(engine.getBestBid()->price == 99);
    assert(engine.getBestAsk()->price == 101);
}

// -----------------------------------------------------------------------------
// Main Runner
// -----------------------------------------------------------------------------
int main() {
    std::cout << "--- Running MatchingEngine Step 3 Unit Tests ---" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    testFullMatch();
    std::cout << "[PASS] Test 1: Full Match" << std::endl;

    testPartialFillAndQueue();
    std::cout << "[PASS] Test 2: Partial Fill & Queue Remainder" << std::endl;

    testMultiLevelSweep();
    std::cout << "[PASS] Test 3: Multi-Level Price Sweep" << std::endl;

    testTimePriorityExecution();
    std::cout << "[PASS] Test 4: Time Priority Execution (FIFO)" << std::endl;

    testNoCross();
    std::cout << "[PASS] Test 5: No Cross / Book Queueing" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "\n--- All Step 3 Unit Tests Passed Successfully! (" 
              << duration << " us) ---" << std::endl;

    return 0;
}