#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "RingBuffer.h"
#include "AudioFormat.h"
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define BUFF_SIZE 4096 
#define BIT_DEPTH 8 


namespace pipeline {

int play_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {

    RingBuffer<sample_type> *queue = (RingBuffer<sample_type> *)userData;

    if (queue->empty()) {
        return paContinue;
    }

    // TODO refactor audioqueue to contain the format and a ringbuffer 
    audio::AudioFormat format(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    short *output = (short *)outputBuffer;    

    for (int i = 0; i < format.bufferSize; i++)
    {
        for (int j = 0; j < format.channels; j++)
        {
            sample_type sample = queue->pop();
            *(output + i * format.channels + j) = sample;
        }
    }

    return paContinue;
}


template <typename T>
class PAInputNode : protected Node {
public:
    PAInputNode();
    ~PAInputNode();
    void update();
     
protected:

};

}