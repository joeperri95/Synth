#include "PASinkNode.h"

namespace pipeline {

int play_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {

    
    audio::AudioQueue<sample_type> *queue = (audio::AudioQueue<sample_type> *)userData;

    if (queue == nullptr) {
        return paContinue;
    }

    audio::AudioFormat format = queue->getFormat();

    sample_type *output = (sample_type *)outputBuffer;    

    for (int i = 0; i < format.bufferSize; i++)
    {
        for (int j = 0; j < format.channels; j++)
        {
            if (queue->empty()) {
                sample_type sample = 0;
                *(output + i * format.channels + j) = 0.5 * sample;
            } else {
                sample_type sample = queue->pop();
                *(output + i * format.channels + j) = 0.5 * sample;
            }
        }
    }

    return paContinue;
}

void PASinkNode::onStateChanged(std::map<std::string, AudioParameter> /*newState*/, void */*args*/) {}
PASinkNode::PASinkNode(NodeID node, AttrID inputID) {
    this->_id = node;
    this->numInputs = 1;
    this->numOutputs = 0;
    this->inputs[inputID] = nullptr;
    this->inputID = inputID;
    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    this->queue = std::make_shared<audio::AudioQueue<sample_type>>();
    this->queue->setFormat(format);
    this->queue->setQueue(this->inputs[inputID]);
    
    AudioDevice &dev = AudioDevice::instance();
    dev.initialize();
    this->stream = dev.addOutputStream(this->format, play_callback, (void *) this->queue.get());
    dev.startStream(this->stream);
}

PASinkNode::~PASinkNode() {}

void PASinkNode::update() {}

void PASinkNode::onInputChanged(AttrID attr) {
    this->queue->setQueue(this->inputs[attr]);
}
     
}