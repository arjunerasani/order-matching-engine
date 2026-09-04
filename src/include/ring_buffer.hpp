#pragma once
#include "order.hpp"

#include <array>
#include <atomic>

// continguous circular memory (array) with head and tail pointers makes this memory efficient
// and allows the cache to return much faster leading to faster times (takes out dynamic heap allocation)
class ringBuffer {
private:
    // this is the actual buffer being able to hold 4096 orders
    std::array<order, 4096> buffer;
    int bufferSize = 4096;

    // these are the head and tail of the buffer
    // they are atomic because i don't want an incoming order to interrupt them mid-operation
    // also it acts as a safety protocol against optimizations
    // head is the place where the new item will be stored
    // tail is the oldest item, the one to be removed next

    // extra note the alignas(64) forces each atomic into its own cache line so the producer writing head and
    // the consumer writing the tail stop invalidating each other's caches causing slower lookups
    alignas(64) std::atomic<uint64_t> head = 0;
    alignas(64) std::atomic<uint64_t> tail = 0;

public:
    // method for pushing the order into the buffer
    bool tryPush(order& newOrder);

    // method for getting the order out of the buffer
    bool tryPop(order& poppedOrder);

    int getBufferSize();
};