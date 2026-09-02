#pragma once
#include "matching_engine.hpp"

#include <chrono>
#include <vector>

class benchmark {
private:
    matchingEngine engine;

public:
    benchmark() = default;

    std::vector<order> orderGenerator();

    void benchmarkTimes();
};