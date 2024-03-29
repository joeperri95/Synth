#include "MixerNode.h"

namespace pipeline {

MixerNode::MixerNode(NodeID id, AttrID input1ID, AttrID input2ID, AttrID outputID){
    this->_id = id;
    this->numInputs = 2;
    this->numOutputs = 1;
    this->input1ID = input1ID;
    this->input2ID = input2ID;
    this->outputID = outputID;
    this->inputs[input1ID] = nullptr;
    this->inputs[input2ID] = nullptr;
    this->outputs[outputID] = nullptr;
    done = false;

    this->format = audio::AudioFormat(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    this->inputQueues[input1ID] = std::make_shared<audio::AudioQueue<sample_type>>();
    this->inputQueues[input1ID]->setFormat(format);
    this->inputQueues[input1ID]->setQueue(this->inputs[input1ID]);

    this->inputQueues[input2ID] = std::make_shared<audio::AudioQueue<sample_type>>();
    this->inputQueues[input2ID]->setFormat(format);
    this->inputQueues[input2ID]->setQueue(this->inputs[input2ID]);

    this->output = std::make_shared<audio::AudioQueue<sample_type>>();
    this->output->setFormat(format);
    this->output->setQueue(this->outputs[outputID]);
    
    this->updateThread = std::thread(MixerNode::update, this); 
}

MixerNode::~MixerNode(){
    this->done = true;
    this->updateThread.join();
}

void MixerNode::update(MixerNode *self) {
    sample_type sample1;
    sample_type sample2;
    sample_type sample;

    int period_us = static_cast<int>(1000000.0f * (self->format.bufferSize) / (self->format.channels * self->format.sampleRate));
    period_us = self->format.getPeriodMicroSeconds();

    while(!self->done) {

        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);

        for(int i = 0; i < self->format.bufferSize; i++) {
            if(!self->inputQueues[self->input1ID]->empty()) {
                sample1 = self->inputQueues[self->input1ID]->pop();
            } else {
                sample1 = 0.0f;
            }
            if(!self->inputQueues[self->input2ID]->empty()) {
                sample2 = self->inputQueues[self->input2ID]->pop();
            } else {
                sample2 = 0.0f;
            }

            sample = sample1 * 0.5 + sample2 * 0.5;

            if(!self->output->full()) {
                self->output->push(sample);
            }
        }
        std::this_thread::sleep_until(sleep);
    }
} 
 
void MixerNode::onInputChanged(AttrID attr) {
    if(this->outputs.find(attr) != this->outputs.end()) {
        spdlog::debug("MixerNode::onInputChanged changing output");
        this->output->setQueue(this->outputs[attr]);
    }
    else if(this->inputs.find(attr) != this->inputs.end()) {
        spdlog::debug("MixerNode::onInputChanged changing input for attr {}", attr);
        this->inputQueues[attr]->setQueue(this->inputs[attr]);
    }
}

extern "C" {
    int build_node(int id, int nextAttrId, Node ** node) {
        *node = new MixerNode(id, nextAttrId, nextAttrId + 2, nextAttrId + 1);
        return nextAttrId + 3;
    }   
}

} // pipeline
