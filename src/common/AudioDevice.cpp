#include "AudioDevice.h"
#include <iostream>

AudioDevice &AudioDevice::instance() {
    static AudioDevice instance_;
    return instance_;
}

AudioDevice::AudioDevice() :  nextStreamID(0), _mutex(), state(AudioDeviceState::IDLE), streams() {
}

AudioDevice::~AudioDevice(){
}

void AudioDevice::initialize() {
    std::lock_guard<std::mutex> lock(this->_mutex);
    if (this->state != INITIALIZED) {
        Pa_Initialize();
        state = AudioDeviceState::INITIALIZED;
    }
}

void AudioDevice::terminate() {
    std::lock_guard<std::mutex> lock(this->_mutex);
    if (this->state == INITIALIZED) {
        Pa_Terminate();
        state = AudioDeviceState::IDLE;
    }
}

StreamID AudioDevice::addInputStream(audio::AudioFormat &format, PaStreamCallback callback, void *data) {
    std::lock_guard<std::mutex> lock(this->_mutex);
    if (this->state == INITIALIZED) {
        StreamID ret = nextStreamID;
        this->streams[nextStreamID++] = nullptr;
        Pa_OpenDefaultStream(&(this->streams[ret]), format.channels, 0, paInt16, format.sampleRate, format.bufferSize, callback, data);
        return ret;
    }
    else {
        std::cerr << "Cannot add stream, not initialized" << std::endl;
        return -1;
    }
}

StreamID AudioDevice::addOutputStream(audio::AudioFormat &format, PaStreamCallback callback, void *data) {
    std::lock_guard<std::mutex> lock(this->_mutex);
    if (this->state == INITIALIZED) {
        StreamID ret = nextStreamID;
        this->streams[nextStreamID++] = nullptr;
        Pa_OpenDefaultStream(&(this->streams[ret]), 0, format.channels, paInt16, format.sampleRate, format.bufferSize, callback, data);
        return ret; 
    }
    else {
        std::cerr << "Cannot add stream, not initialized" << std::endl;
        return -1;
    }
}

void AudioDevice::removeStream(StreamID stream) {
    std::lock_guard<std::mutex> lock(this->_mutex);
    Pa_CloseStream(this->streams[stream]);
    this->streams.erase(stream);
}

void AudioDevice::startStream(StreamID stream) {
    std::lock_guard<std::mutex> lock(this->_mutex);
    Pa_StartStream(this->streams[stream]);
}

void AudioDevice::stopStream(StreamID stream) {
    std::lock_guard<std::mutex> lock(this->_mutex);
    Pa_StopStream(this->streams[stream]);
}

