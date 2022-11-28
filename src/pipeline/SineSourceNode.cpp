#include "SineSourceNode.h"

namespace pipeline {

SineSourceNode::SineSourceNode(NodeID id, AttrID outputID){
    this->_id = id;
    this->numInputs = 0;
    this->numOutputs = 1;
    this->outputID = outputID;
    this->outputs[outputID] = nullptr;
    this->frequency = 220.0f;
    done = false;

    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    this->output = std::make_shared<audio::AudioQueue<sample_type>>();
    this->output->setFormat(format);
    this->output->setQueue(this->outputs[outputID]);
    
    this->updateThread = std::thread(SineSourceNode::update, this); 
}

SineSourceNode::~SineSourceNode(){
    this->done = true;
    this->updateThread.join();
}

void SineSourceNode::update(SineSourceNode *self) {
    int period = static_cast<int>(1000.0f / self->format.sampleRate);
    double omegaNormalized = 2 * M_PI * self->frequency / self->format.sampleRate;
    int i = 0;

    while(!self->done) {
        sample_type sample = static_cast<int> (65535 / 2 * sin((i++) * omegaNormalized));
        if (!self->output->full()) {
            self->output->push(sample);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(period));
    }
} 
 
void SineSourceNode::onInputChanged(AttrID attr) {
    if(this->outputs.find(attr) != this->outputs.end()) {
        this->output->setQueue(this->outputs[attr]);
    }
}

} // pipeline
