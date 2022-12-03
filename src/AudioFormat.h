#pragma once
#include <ostream>

#define SAMPLE_RATE 44100
#define CHANNELS 1
#define BUFF_SIZE 4096 
#define BIT_DEPTH 16 

namespace audio {

struct AudioFormat
{
    AudioFormat() : channels(0), sampleRate(0), bitDepth(0), bufferSize(0)
    {}

    AudioFormat(int channels, int sampleRate, int bitDepth, int bufferSize)
                : channels(channels), sampleRate(sampleRate), bitDepth(bitDepth), bufferSize(bufferSize)
    {}

    ~AudioFormat(){}

    int channels;   // Number of audio channels
    int sampleRate; // Sampling rate of the audio
    int bitDepth;   // Number of bits per sample
    int bufferSize; // Number of frames per buffer

    // Calculate the number of bytes in an audio buffer with this format
    int getBytesPerBuffer(){
        return (this->bitDepth >> 3) *this->channels * this->bufferSize;
    }

    // Return the number of samples in an audio buffer with this format 
    int getSamplesPerBuffer(){
        return this->channels * this->bufferSize;
    }

    // Return the number of frames in an audio buffer with this format 
    int getFramesPerBuffer(){
        return this->bufferSize;
    }

    friend std::ostream& operator<<(std::ostream& out, AudioFormat& audio) {
        out << "Channels: " << audio.channels << std::endl;
        out << "Sample Rate: " << audio.sampleRate << std::endl;
        out << "Bit Depth: " << audio.bitDepth << std::endl;
        out << "Buffer Size: " << audio.bufferSize << std::endl;
        return out;
    }

};

} // namespace audio