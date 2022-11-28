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
    while(!self->done) {
        if(!self->input->empty()) {
            sample_type sample = self->input->pop();
            if(!self->output->full()) {
                self->output->push(sample * self->volume);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0f / self->format.sampleRate)));
    }
} 
 
void VolumeNode::onInputChanged(AttrID attr) {
    if(this->outputs.find(attr) != this->outputs.end()) {
        this->output->setQueue(this->outputs[attr]);
    }
    else if(this->inputs.find(attr) != this->inputs.end()) {
        this->input->setQueue(this->inputs[attr]);
    }
}

} // pipeline
