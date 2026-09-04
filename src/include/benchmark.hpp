#pragma once
#include "matching_engine.hpp"
#include "ring_buffer.hpp"

#include <chrono>
#include <vector>

class benchmark {
private:
    matchingEngine engine;

public:
    benchmark() = default;

    std::vector<order> orderGenerator();

    void producerTask(std::vector<order>& orders, ringBuffer& rb);

    void consumerTask(ringBuffer& rb);

    void benchmarkTimes();
};