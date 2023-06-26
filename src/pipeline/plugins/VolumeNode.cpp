#include "VolumeNode.h"

namespace pipeline {

VolumeNode::VolumeNode(NodeID id, AttrID inputID, AttrID outputID){
    this->_id = id;
    this->numInputs = 1;
    this->numOutputs = 1;
    this->inputID = inputID;
    this->outputID = outputID;
    this->volume = 0.5;
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
    
    this->updateThread = std::thread(VolumeNode::update, this); 
}

VolumeNode::~VolumeNode(){
    this->done = true;
    this->updateThread.join();
}

void VolumeNode::update(VolumeNode *self) {
    int period_us = self->format.getPeriodMicroSeconds();

    while(!self->done) {
        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);
        for (int i = 0; i < self->format.bufferSize; i++) {
            if(!self->input->empty()) {
                sample_type sample = self->input->pop();

                if(!self->output->full()) {
                    self->output->push(sample * self->volume);
                } 
            } else {
                //spdlog::debug("VolumeNode::update input queue is empty and isQueueValid == {}", self->input->isQueueValid());
            }
        }
        std::this_thread::sleep_until(sleep);
    }
} 
 
void VolumeNode::onInputChanged(AttrID attr) {
    spdlog::debug("VolumeNode::onInputChanged");
    if(this->outputs.find(attr) != this->outputs.end()) {
        this->output->setQueue(this->outputs[attr]);
    }
    else if(this->inputs.find(attr) != this->inputs.end()) {
        this->input->setQueue(this->inputs[attr]);
    }
    else {
        spdlog::warn("Cannot find attr in inputs or outputs");
    }
}

void VolumeNode::onStateChanged(std::map<std::string, AudioParameter> newState, void */*arg*/) {
    auto it = newState.find("volume");
    if(it != newState.end()) {
        AudioParameter param = newState["volume"];
        if(param.getType() != AudioParameterType::TYPE_FLOAT) {
            spdlog::warn("VolumeNode::onStateChange did not receive a float parameter");
        } else {
            spdlog::info("VolumeNode::onStateChange changing volume parameter");
            this->volume = param.getParamFloat();
        }
    } else {
        spdlog::warn("VolumeNode::onStateChange did not find volume parameter");
    }
}

extern "C" {
    int build_node(int id, int nextAttrId, Node ** node) {
        *node = new VolumeNode(id, nextAttrId, nextAttrId + 1);
        return nextAttrId + 2;
    }   
}

} // pipeline
