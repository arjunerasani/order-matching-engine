#pragma once
#include "order.hpp"

#include <array>
#include <atomic>

// continguous circular memory with head and tail pointers makes this memory efficient
// and allows the cache to return much faster leading to faster times (takes out dynamic heap allocation)
class ringBuffer {
private:
    // this is the actual buffer being able to hold ~4100 orders
    std::array<order, 4096> buffer;

    // these are the head and tail of the buffer
    // they are atomic because i don't want an incoming order to interrupt them mid-operation
    // also it acts as a safety protocol against optimizations
    std::atomic<uint64_t> head;
    std::atomic<uint64_t> tail;

public:
    bool tryPush(order& newOrder) {

    }

    bool tryPop(order& newOrder) {
        
    } 
};