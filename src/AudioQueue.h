#pragma once

#include <mutex>
#include <queue>
#include "AudioFormat.h"
#include "AudioBuffer.h"

namespace audio {

template<typename T>
struct AudioQueue {
    AudioQueue() : _mutex(), _queue(), format() {}
    std::mutex _mutex;
    std::deque<AudioBuffer<T>> _queue;
    AudioFormat format;
};

}