#include "ring_buffer.hpp"

bool ringBuffer::tryPush(order& newOrder) {
    // this checks for a full buffer
    uint64_t nextHeadPos = (head + 1) % bufferSize;

    // if the buffer is full we can't push a new order
    // so return false
    if (nextHeadPos == tail) {
        return false;
    }

    // otherwise push the new order in and
    // advance the head
    buffer[head] = newOrder;
    head = nextHeadPos;

    return true;
}

bool ringBuffer::tryPop(order& poppedOrder) {
    // nothing to pop so return false
    if (head == tail) {
        return false;
    }

    // assign the buffer[tail] to the var
    // so it can be accessed later
    poppedOrder = buffer[tail];

    // move tail forward also wraps around if necessary
    tail = (tail + 1) % bufferSize;

    return true;
} 

int ringBuffer::getBufferSize() {
    return bufferSize;
}