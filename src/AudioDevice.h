#pragma once

#include "portaudio.h"
#include "AudioFormat.h"
#include <vector> 
#include <map> 
#include <memory> 
#include <iostream> 

typedef int StreamID;

typedef enum {
  IDLE,
  INITIALIZED,
} AudioDeviceState;

class AudioDevice {
public:
    AudioDevice();
    ~AudioDevice();

    void initialize();
    void terminate();

    StreamID addInputStream(audio::AudioFormat &format, PaStreamCallback callback, void * data);
    StreamID addOutputStream(audio::AudioFormat &format, PaStreamCallback callback, void * data);
    void removeStream(StreamID stream);
    void startStream(StreamID stream);
    void stopStream(StreamID stream);

private:
    // PaStream is a typedef of void so smart pointers cannot be used
    std::map<StreamID, PaStream*> streams;
    AudioDeviceState state;
    StreamID nextStreamID;
};