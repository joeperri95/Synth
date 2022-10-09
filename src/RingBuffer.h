#pragma once

#include <atomic>
#include <memory>

template <typename T>
class RingBuffer {
public:
    RingBuffer();

    void push(sample_type sample);
    
    // Return the front of the queue
    sample_type pop();

    // If the queue is empty return 0
    // Otherwise pop()
    sample_type pop_or_zero();

    bool empty();
    bool full();
    int size();
    int capacity();

private:
    std::unique_ptr<std::shared_ptr<T>> _read;
    std::unique_ptr<std::shared_ptr<T>> _write;
    std::unique_ptr<std::shared_ptr<T>[]> _buffer;
    int _capacity;
};