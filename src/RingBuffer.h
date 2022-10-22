#pragma once

#include <memory>

template <typename T>
concept Numeric = std::is_numeric<T>::value;

template <Numeric T>
class RingBuffer {
public:
    RingBuffer(int capacity);
    virtual ~RingBuffer() = 0

    // Push a new sample to the end of the queue. 
    // If the queue is full replace the last element with the new sample
    virtual void push(T sample) = 0;
    
    // Remove the element at front of the queue and return it
    virtual T pop() = 0;

    // If the queue is empty return 0
    // Otherwise pop()
    virtual T pop_or_zero() noexcept = 0;

    virtual bool empty() const = 0;
    virtual bool full() const = 0;
    virtual int size() const = 0;
    virtual int capacity() const = 0;

protected:
    int _capacity;
};