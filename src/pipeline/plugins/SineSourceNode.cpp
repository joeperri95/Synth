#include "SineSourceNode.h"

namespace pipeline {

SineSourceNode::SineSourceNode(NodeID id, AttrID outputID){
    this->_id = id;
    this->numInputs = 0;
    this->numOutputs = 1;
    this->outputID = outputID;
    this->outputs[outputID] = nullptr;
    this->frequency = 440.0f;
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
    int period_us = static_cast<int>(1000000.0f * (self->format.bufferSize) / (self->format.channels * self->format.sampleRate));
    double omegaNormalized = 2 * M_PI * self->frequency / self->format.sampleRate;
    long long i = 0;

    while(!self->done) {
        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);
        omegaNormalized = 2 * M_PI * self->frequency / self->format.sampleRate;
        for (int j = 0; j < self->format.bufferSize; j++) {
            sample_type sample = static_cast<sample_type> (65535 / 2 * sin((i++) * omegaNormalized));
            for(int k = 0; k < self->format.channels; k++) {
                if (!self->output->full()) {
                    self->output->push(sample);
                }
            }
        }
        std::this_thread::sleep_until(sleep);
    }
} 
 
void SineSourceNode::onInputChanged(AttrID attr) {
    if(this->outputs.find(attr) != this->outputs.end()) {
        this->output->setQueue(this->outputs[attr]);
    }
}

void SineSourceNode::onStateChanged(std::map<std::string, AudioParameter> newState, void */*arg*/) {
    auto it = newState.find("frequency");
    if(it != newState.end()) {
        AudioParameter param = newState["frequency"];
        if(param.getType() != AudioParameterType::TYPE_FLOAT) {
            spdlog::warn("SineSourceNode::onStateChange did not receive a float parameter");
        } else {
            spdlog::info("SineSourceNode::onStateChange changing frequency parameter");
            this->frequency = param.getParamFloat();
        }
    } else {
        spdlog::warn("SineSourceNode::onStateChange did not find volume parameter");
    }
}

extern "C" {
    int build_node(int id, int nextAttrId, Node ** node) {
        *node = new SineSourceNode(id, nextAttrId);
        return nextAttrId + 1;
    }   
}

} // pipeline
