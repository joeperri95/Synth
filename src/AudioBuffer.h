#pragma once

#include <vector>
#include "AudioFormat.h"

namespace audio {

// deprecate this and use ringbuffers instead

template<typename T>
class AudioBuffer
{
public:
    AudioBuffer(AudioFormat format, std::vector<T> data) {
        this->format = format;
        this->data = std::move(data);
    }

    AudioBuffer(AudioFormat format) {
        this->format = format;
        this->data = std::vector<T>(format.getSamplesPerBuffer());
    }

    AudioFormat getFormat() const {
        return this->format;
    }

    std::vector<T> getData() const {
        return this->data;
    }

    void setData(std::vector<T> data) {
        std::swap(this->data, data);
    }

private:
    AudioFormat format;
    std::vector<T> data;
};

} // namespace audio