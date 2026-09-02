#include "tests.hpp"
#include "benchmark.hpp"

// -----------------------------------------------------------------------------
// Main Runner
// -----------------------------------------------------------------------------
int main() {
    /*
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

    return 0;*/

    benchmark bm;

    bm.benchmarkTimes();
}