#include "benchmark.hpp"
#include "order_book.hpp"
#include "trade_event.hpp"

#include <random>

// this method just generates an equal number of buy and sell orders at varying prices
std::vector<order> benchmark::orderGenerator() {
    std::vector<order> orders;
    // reserve space for the orders to prevent reallocation during generation
    orders.reserve(1000000);

    // this obtains a random seed from the hardware
    // then uses the mersenne twister engine to generate pseudo random bits
    std::random_device rd;
    std::mt19937 gen(rd());

    // this ensures every number in the execution range has an equal chance of being picked
    // this is for price and quantity
    std::uniform_int_distribution<> priceDistr(95, 105);
    std::uniform_int_distribution<> qtyDistr(1, 100);

    for (uint64_t i = 0; i < 1000000; i++) {
        order newOrder;

        if (i % 2 == 0) {
            newOrder.orderType = orderType::BUY;
        } else {
            newOrder.orderType = orderType::SELL;
        }

        newOrder.orderId = i;
        newOrder.ownerId = 0;
        newOrder.price = priceDistr(gen);
        newOrder.quantity = qtyDistr(gen);
        newOrder.timestamp = i;

        orders.push_back(newOrder);
    }

    return orders;
};

// this method is for benchmarking the amount of time the engine takes to go through all 1,000,000 orders
void benchmark::benchmarkTimes() {
    std::vector<order> orders = orderGenerator();

    // this starts the timer and sees how long it takes for the orders to be processed
    auto startTime = std::chrono::high_resolution_clock::now();

    for (auto& order: orders) {
        engine.submitOrder(order);
    }

    // get the duration this ran for
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // then compute the metrics
    double seconds = duration / 1000.0;
    double opsPerSec = 1000000.0 / seconds;

    std::cout << "--- Benchmark Results (1,000,000 Orders) ---" << std::endl;
    std::cout << "Total Time: " << duration << " ms" << std::endl;
    std::cout << "Throughput: " << opsPerSec << " orders/sec" << std::endl;
};