#pragma once

#include "common/AudioBuffer.h"
#include "AudioFormat.h"
#include "AudioQueue.h"
#include <cmath>

namespace audio { namespace effects {

template<typename T>
AudioBuffer<T> create_sine(AudioFormat &format, int frequency)
{
    std::vector<T> ret;
    ret.resize(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++)
    {
        for(int j = 0; j < format.channels; j++)
        {
            ret.at(i * format.channels + j) = (short) (INT16_MAX * std::sin(2 * M_PI * frequency / (1.0 * format.sampleRate) * i));        
        }
    }

    return AudioBuffer(format, ret);
}

template<typename T>
AudioBuffer<T> volume(AudioBuffer<T> input, float volume)
{    
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++)
    {
        for(int j = 0; j < format.channels; j++)
        {
            ret.at(i * format.channels + j) = volume * inData.at(i * format.channels + j);
        }
    }

    return AudioBuffer(format, ret);
}

template<typename T>
AudioBuffer<T> tremolo(AudioBuffer<T> input, float frequency)
{
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++)
    {
        for(int j = 0; j < format.channels; j++)
        {
            ret.at(i * format.channels + j) = (short) (std::sin(2 * M_PI * frequency / (1.0 * format.sampleRate) * i) * inData.at(i * format.channels + j));
        }
    }

    return AudioBuffer(format, ret);
}

template<typename T>
AudioBuffer<T> vibrato(AudioBuffer<T> input, float frequency, int depth)
{    
    // need to check depth
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++)
    {
        for(int j = 0; j < format.channels; j++)
        {
            if((i < depth) || (i + depth > format.bufferSize)) {
                ret.at(i * format.channels + j) = inData.at(i * format.channels + j);
            }
            else {
                ret.at(i * format.channels + j) = inData.at(static_cast<int>(i * format.channels + j + depth * sin(2 * M_PI * frequency / format.sampleRate * i)));
            }
        }
    }

    return AudioBuffer(format, ret);
}


template<typename T>
AudioBuffer<T> flanger(AudioBuffer<T> input, int depth, float decay, float frequency)
{    
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++) {
        for(int j = 0; j < format.channels; j++) {
            ret.at(i * format.channels + j) = (1-decay) * inData.at(static_cast<int>(i * format.channels + j));
            if(i > depth) {
                ret.at(i * format.channels + j) += decay * inData.at(static_cast<int>((i - depth * sin(2 * M_PI * frequency / format.sampleRate * i)) * format.channels + j));
            }
            else {
                ret.at(i * format.channels + j) += decay * inData.at(static_cast<int>(i * format.channels + j));
            }
        }
    }

    return AudioBuffer(format, ret);
}


template<typename T>
AudioBuffer<T> reverb(AudioBuffer<T> input, int depth, float decay)
{    
    #define THRESH 0.1
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++) {
        for(int j = 0; j < format.channels; j++) {
            ret.at(i * format.channels + j) = inData.at(static_cast<int>(i * format.channels + j));
            double dc = decay;
            int k = 0;
            while(dc > THRESH && (depth * k < format.bufferSize)) {
                int index = (i - (depth * k++)) * format.channels + j;
                index = index > 0 ? index : 0;
                ret.at(i * format.channels + j) += dc * inData.at(index);
                dc *= decay;
            }
        }
    }

    return AudioBuffer(format, ret);
}


/*
    -1.0 for full left channel
      0  for no adjustment
    +1.0 for full right channel
*/
template<typename T>
AudioBuffer<T> stereo_pan(AudioBuffer<T> input, float pan) {
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());
    bool leftRight = false;

    if (format.channels != 2) {
        return input;
    }

    float left, right;

    if (pan == 0) {
        left = 1.0;
        right = 1.0;
    }
    else if(pan < 0) {
        pan = abs(pan);
        left = pan;
        right = (1.0 - pan);
    } 
    else if (pan > 0) {
        pan = abs(pan);
        left = (1.0 - pan);
        right = pan;
    }

    for(int i = 0; i < format.bufferSize; i++) {
        int index = 2 * i * format.channels;
        ret.at(index) = left * inData.at(index);
        ret.at(index + 1) = right * inData.at(index + 1);
    }

}

template<typename T>
AudioBuffer<T> hard_limiter(AudioBuffer<T> input, float threshold)
{    
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++) {
        for(int j = 0; j < format.channels; j++) {
            int index = i * format.channels + j;
            if (inData.at(index) > threshold) {
                ret.at(index) = threshold;    
            }
            else {
                ret.at(index) = inData.at(index);
            }
        }
    }

    return AudioBuffer(format, ret);
}

template<typename T>
AudioBuffer<T> delay(AudioBuffer<T> input, int depth, double decay)
{    
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++) {
        for(int j = 0; j < format.channels; j++) {
            ret.at(i * format.channels + j) = (1-decay) * inData.at(static_cast<int>(i * format.channels + j));
            if(i > depth) {
                ret.at(i * format.channels + j) += decay * inData.at(static_cast<int>((i - depth) * format.channels + j));
            }
            else {
                ret.at(i * format.channels + j) += decay * inData.at(static_cast<int>(i * format.channels + j));
            }
        }
    }

    return AudioBuffer(format, ret);
}

template<typename T>
AudioBuffer<T> average_filter(AudioBuffer<T> input, int n)
{    
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++) {
        for(int j = 0; j < format.channels; j++) {

            int sum = 0;
            for(int k = i - n / 2; k < i + n; k++) {
                if(k > 0 && k < format.bufferSize) {
                    sum += inData.at(k * format.channels + j);
                } else {
                    sum += inData.at(i * format.channels + j);
                }
            }
            
            ret.at(i * format.channels + j) = sum / n;
        }
    }

    return AudioBuffer(format, ret);
}


template<typename T>
void splitter(AudioQueue<T> &input, AudioQueue<T> &output1, AudioQueue<T> &output2)
{    
    if(!input._queue.empty()) {
        AudioBuffer<T> sample = input._queue.front();
        input._queue.pop_front();
        output1._queue.push_back(sample);
        output2._queue.push_back(sample);
    }
}

template<typename T>
AudioBuffer<T> mixer(AudioBuffer<T> input1, AudioBuffer<T> input2, double balance)
{    
    AudioFormat format = input1.getFormat();
    AudioFormat format2 = input2.getFormat();

    std::vector<T> data1 = input1.getData();
    std::vector<T> data2 = input2.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize; i++) {
        for(int j = 0; j < format.channels; j++) {
            T sample1 = data1.at(i * format.channels + j);
            T sample2 = data1.at(i * format.channels + j);
            ret.at(i * format.channels + j) = sample1 * balance + sample2 * (1 - balance);
        }
    }

    return AudioBuffer(format, ret);
}


template<typename T>
AudioBuffer<T> bitcrush(AudioBuffer<T> input)
{    
    AudioFormat format = input.getFormat();
    std::vector<T> inData = input.getData();
    std::vector<T> ret(format.getSamplesPerBuffer());

    for(int i = 0; i < format.bufferSize - 8; i+=8) {
        for(int j = 0; j < format.channels; j++) {
            T curr = inData.at((i) * format.channels + j);
            ret.at(i * format.channels + j) = curr;
            ret.at((i + 1) * format.channels + j) = curr;
            ret.at((i + 2) * format.channels + j) = curr;
            ret.at((i + 3) * format.channels + j) = curr;
            ret.at((i + 4) * format.channels + j) = curr;
            ret.at((i + 5) * format.channels + j) = curr;
            ret.at((i + 6) * format.channels + j) = curr;
            ret.at((i + 7) * format.channels + j) = curr;
        }
    }

    return AudioBuffer(format, ret);
}


}} // audio::effects
