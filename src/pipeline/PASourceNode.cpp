#include "PASourceNode.h"

namespace pipeline {

int record_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{

    audio::AudioQueue<sample_type> *queue = (audio::AudioQueue<sample_type> *)userData;

    if (queue == nullptr) {
        return paContinue;
    }

    audio::AudioFormat format = queue->getFormat();

    sample_type *input = (sample_type *)inputBuffer;    

    for (int i = 0; i < format.bufferSize; i++)
    {
        for (int j = 0; j < format.channels; j++)
        {
            short sample = *(input + i * format.channels + j);
            queue->push(sample);
        }
    }

    return paContinue;
}

void PASourceNode::onStateChanged(std::map<std::string, AudioParameter> /*newState*/, void */*args*/) {}
PASourceNode::PASourceNode(NodeID node, AttrID outputID) {
    this->_id = node;
    this->numInputs = 0;
    this->numOutputs = 1;
    this->outputs[outputID] = nullptr;
    this->outputID = outputID;
    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);
    
    this->queue = std::make_shared<audio::AudioQueue<sample_type>>();
    this->queue->setFormat(format);
    this->queue->setQueue(this->outputs[outputID]);

    AudioDevice &dev = AudioDevice::instance();
    dev.initialize();
    this->stream = dev.addInputStream(this->format, record_callback, (void *) this->queue.get());
    dev.startStream(this->stream);
}

PASourceNode::~PASourceNode() {}

void PASourceNode::update() {}

void PASourceNode::onInputChanged(AttrID attr) {
    this->queue->setQueue(this->outputs[attr]);
}

}