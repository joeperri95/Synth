#include "SplitterNode.h"

namespace pipeline {

SplitterNode::SplitterNode(NodeID id, AttrID inputID, AttrID output1ID, AttrID output2ID){
    this->_id = id;
    this->numInputs = 1;
    this->numOutputs = 2;
    this->inputID = inputID;
    this->output1ID = output1ID;
    this->output2ID = output2ID;
    this->outputs[output1ID] = nullptr;
    this->outputs[output2ID] = nullptr;
    this->inputs[inputID] = nullptr;
    done = false;

    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    this->outputQueues[output1ID] = std::make_shared<audio::AudioQueue<sample_type>>();
    this->outputQueues[output1ID]->setFormat(format);
    this->outputQueues[output1ID]->setQueue(this->outputs[output1ID]);

    this->outputQueues[output2ID] = std::make_shared<audio::AudioQueue<sample_type>>();
    this->outputQueues[output2ID]->setFormat(format);
    this->outputQueues[output2ID]->setQueue(this->outputs[output2ID]);

    this->input = std::make_shared<audio::AudioQueue<sample_type>>();
    this->input->setFormat(format);
    this->input->setQueue(this->inputs[inputID]);
    
    this->updateThread = std::thread(SplitterNode::update, this); 
}

SplitterNode::~SplitterNode(){
    this->done = true;
    this->updateThread.join();
}

void SplitterNode::update(SplitterNode *self) {
    sample_type sample;

    int period_us = static_cast<int>(1000000.0f * (self->format.bufferSize) / (self->format.channels * self->format.sampleRate));
    period_us = self->format.getPeriodMicroSeconds();

    while(!self->done) {

        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);

        for(int i = 0; i < self->format.bufferSize; i++) {

            if(!self->input->empty()) {
               sample = self->input->pop(); 
            } else {
               sample = 0.0f;
            }

            if(!self->outputQueues[self->output1ID]->full()) {
                self->outputQueues[self->output1ID]->push(sample);
            }

            if(!self->outputQueues[self->output2ID]->full()) {
                self->outputQueues[self->output2ID]->push(sample);
            }
        }
        std::this_thread::sleep_until(sleep);
    }
} 
 
void SplitterNode::onInputChanged(AttrID attr) {
    if(this->outputs.find(attr) != this->outputs.end()) {
        spdlog::debug("SplitterNode::onInputChanged changing output");
        this->outputQueues[attr]->setQueue(this->outputs[attr]);
    }
    else if(this->inputs.find(attr) != this->inputs.end()) {
        spdlog::debug("SplitterNode::onInputChanged changing input for attr {}", attr);
        this->input->setQueue(this->inputs[attr]);
    }
}

extern "C" {
    int build_node(int id, int nextAttrId, Node ** node) {
        *node = new SplitterNode(id, nextAttrId + 1, nextAttrId, nextAttrId + 2);
        return nextAttrId + 3;
    }   
}

} // pipeline
