#pragma once

#include <mutex>
#include <queue>
#include "AudioFormat.h"
#include "common/RingBuffer.h"

#include <iostream>

namespace audio {

template<Numeric T>
class AudioQueue {
public:
    AudioQueue() : _mutex(), _queue(nullptr), _format() {}

    bool isQueueValid() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        return this->_queue != nullptr;
    }

    void setQueue(std::shared_ptr<RingBuffer<T>> queue) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        this->_queue = queue;
    };

    void setFormat(AudioFormat format) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        this->_format = format;
    }

    std::shared_ptr<RingBuffer<T>> getQueue() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        return this->_queue;
    }

    AudioFormat getFormat() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        return this->_format;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        if (this->_queue.get() == nullptr) {
            return true;
        }
        return this->_queue->empty();
    }

    bool full() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        if (this->_queue.get() == nullptr) {
            return true;
        }
        return this->_queue->full();
    }

    void push(T t) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        if (this->_queue.get() != nullptr) {
            this->_queue->push(t);
        }
    }

    T pop() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        if (this->_queue.get() == nullptr) {
            return T();
        }
        return this->_queue->pop();
    }

    friend std::ostream& operator<<(std::ostream& out, AudioQueue& audio) {
        if (audio._queue.get() == nullptr) {
            out << "Queue is null" << std::endl;
        } else {
            out << "Capacity: " << audio._queue->capacity() << std::endl;
            out << "Elements: " << audio._queue->size() << std::endl;
        }
        return out;
    }

private:
    std::mutex _mutex;
    std::shared_ptr<RingBuffer<T>> _queue;
    AudioFormat _format;
};


}
