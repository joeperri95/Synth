
#include "SquareSourceNode.h"

namespace pipeline {

SquareSourceNode::SquareSourceNode(NodeID id, AttrID outputID){
    this->_id = id;
    this->numInputs = 0;
    this->numOutputs = 1;
    this->outputID = outputID;
    this->outputs[outputID] = nullptr;
    this->frequency = 220.0f;
    this->done = false;
    this->validQueue = false;

    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    this->output = std::make_shared<audio::AudioQueue<sample_type>>();
    this->output->setFormat(format);
    this->output->setQueue(this->outputs[outputID]);
    
    this->updateThread = std::thread(SquareSourceNode::update, this); 
}

SquareSourceNode::~SquareSourceNode(){
    this->done = true;
    this->updateThread.join();
}

void SquareSourceNode::update(SquareSourceNode *self) {

    int i = 0;
    double index = 0;

    while(!self->done) {
        double omegaNormalized = 2 * M_PI * self->frequency / self->format.sampleRate;
        int period_us = static_cast<int>(1000000.0f * (self->format.bufferSize) / (self->format.sampleRate * self->format.channels));

        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);

        for (int k = 0; k < self->format.bufferSize; k++) {
            if (self->validQueue) {
                index = (omegaNormalized * i++);
                if(index >= (2 * M_PI)) {
                    i = 0;
                    index = 0;
                }

                sample_type sample = static_cast<sample_type> (65535 / 2 * (index > M_PI));
                for(int j = 0; j < self->format.channels; j++) {
                    if (!self->output->full()) {
                        self->output->push(sample);
                    }
                }
            } else {
                index = 0;
                i = 0;
            }
        }
        std::this_thread::sleep_until(sleep);
        std::chrono::duration<double, std::milli> dur = std::chrono::high_resolution_clock::now() - now;
        if (dur.count() > 43.0) {
            spdlog::warn("Square source time taken {}", dur.count());
        }
    }
} 
 
void SquareSourceNode::onInputChanged(AttrID attr) {
    if(this->outputs.find(attr) != this->outputs.end()) {
        this->output->setQueue(this->outputs[attr]);
        this->validQueue = this->outputs[attr] != nullptr;
    }
}

void SquareSourceNode::onStateChanged(std::map<std::string, AudioParameter> newState, void */*arg*/) {
    auto it = newState.find("frequency");
    if(it != newState.end()) {
        AudioParameter param = newState["frequency"];
        if(param.getType() != AudioParameterType::TYPE_FLOAT) {
            spdlog::warn("SquareSourceNode::onStateChange did not receive a float parameter");
        } else {
            spdlog::info("SquareSourceNode::onStateChange changing frequency parameter");
            this->frequency = param.getParamFloat();
        }
    } else {
        spdlog::warn("SquareSourceNode::onStateChange did not find volume parameter");
    }
}

extern "C" {
    int build_node(int id, int nextAttrId, Node ** node) {
        *node = new SquareSourceNode(id, nextAttrId);
        return nextAttrId + 1;
    }   
}

} // pipeline
