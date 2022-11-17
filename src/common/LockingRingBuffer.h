#pragma once

#include "RingBuffer.h"
#include <mutex>

template <Numeric T>
class LockingSPSCRingBuffer : public RingBuffer<T> {
public:
    explicit LockingSPSCRingBuffer(int capacity) :
        _head(0),
        _tail(0),
        _buffer(std::unique_ptr<T[]>(new T[capacity])),
        _full(false),
        _capacity(capacity),
        _mutex()
    {}

    void push(T sample) override {
        std::lock_guard<std::mutex> lock(_mutex);
        _buffer[_tail] = sample;

        if(_full) {
            _head = (_head + 1) % _capacity;
        }

        _tail = (_tail + 1) % _capacity;
        _full = _tail == _head; 
    }
    
    T pop() override {
        std::lock_guard<std::mutex> lock(_mutex);
        T sample = _buffer[_head];
        _head = (_head + 1) % _capacity;
        
        return sample;
    };

    T pop_or_zero() noexcept override{
        std::lock_guard<std::mutex> lock(_mutex);
        if (this->empty()) {
            return 0;
        } else {
            T sample = _buffer[_head];
            _head = (_head + 1) % _capacity;

            return sample; 
        }
    }

    bool empty() const override {
        return !_full && (_head == _tail);
    }

    bool full() const override {
        return _full;
    };

    int size() const override {
        int size = _capacity;

        if(!_full)
        {
            if(_head >= _tail)
            {
                size = _head - _tail;
            }
            else
            {
                size = _capacity + _head - _tail;
            }
        }

        return size;
    }

    int capacity() const override {
        return _capacity;
    }

private:
    int _head;
    int _tail;
    std::unique_ptr<T[]> _buffer;
    bool _full;
    int _capacity;
    std::mutex _mutex;
};
