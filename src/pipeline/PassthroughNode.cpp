#include "PassthroughNode.h"

namespace pipeline {

PassthroughNode::PassthroughNode(NodeID id, AttrID inputID, AttrID outputID){
    this->_id = id;
    this->numInputs = 1;
    this->numOutputs = 1;
    this->inputID = inputID;
    this->outputID = outputID;
    this->inputs[inputID] = nullptr;
    this->outputs[outputID] = nullptr;
    done = false;

    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    this->input = std::make_shared<audio::AudioQueue<sample_type>>();
    this->input->setFormat(format);
    this->input->setQueue(this->inputs[inputID]);

    this->output = std::make_shared<audio::AudioQueue<sample_type>>();
    this->output->setFormat(format);
    this->output->setQueue(this->outputs[outputID]);
    
    this->updateThread = std::thread(PassthroughNode::update, this); 
}

PassthroughNode::~PassthroughNode(){
    this->done = true;
    this->updateThread.join();
}

void PassthroughNode::update(PassthroughNode *self) {
    sample_type sample;

    int period_us = self->format.getPeriodMicroSeconds();

    while(!self->done) {

        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);

        for(int i = 0; i < self->format.bufferSize; i++) {
            sample = self->input->pop();

            if(!self->output->full()) {
                self->output->push(sample);
            }
        }
        std::this_thread::sleep_until(sleep);
    }
} 
 
void PassthroughNode::onInputChanged(AttrID attr) {
    if(this->outputs.find(attr) != this->outputs.end()) {
        spdlog::debug("PassthroughNode::onInputChanged changing output");
        this->output->setQueue(this->outputs[attr]);
    } else if(this->inputs.find(attr) != this->inputs.end()) {
        spdlog::debug("PassthroughNode::onInputChanged changing input for attr {}", attr);
        this->input->setQueue(this->inputs[attr]);
    } else {
        spdlog::warn("Cannot find attr in inputs or outputs");
    }
}

} // pipeline
