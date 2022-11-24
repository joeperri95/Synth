#pragma once

#include "portaudio.h"
#include "AudioFormat.h"
#include <vector> 
#include <map> 
#include <memory> 
#include <mutex> 
#include <iostream> 

typedef int StreamID;

typedef enum {
  IDLE,
  INITIALIZED,
} AudioDeviceState;

class AudioDevice {
public:
    static AudioDevice &instance();
    ~AudioDevice();

    void initialize();
    void terminate();

    StreamID addInputStream(audio::AudioFormat &format, PaStreamCallback callback, void * data);
    StreamID addOutputStream(audio::AudioFormat &format, PaStreamCallback callback, void * data);
    void removeStream(StreamID stream);
    void startStream(StreamID stream);
    void stopStream(StreamID stream);

private:
    AudioDevice();
    // PaStream is a typedef of void so smart pointers cannot be used
    std::map<StreamID, PaStream*> streams;
    std::mutex _mutex;
    AudioDeviceState state;
    StreamID nextStreamID;
};