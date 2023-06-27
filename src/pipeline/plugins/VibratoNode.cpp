#include "VibratoNode.h"
#include "common/AudioQueue.h"
#include "pipeline/Node.h"
#include <memory>
#include <iostream>

const int VIBRATO_CAPACITY = 16384;

namespace pipeline {

VibratoNode::VibratoNode(NodeID id, AttrID inputID, AttrID outputID){
    std::cout << "Start of vibrato ctor" << std::endl;
    this->_id = id;
    this->numInputs = 1;
    this->numOutputs = 1;
    this->inputID = inputID;
    this->outputID = outputID;
    this->depth = 40;
    this->frequency = 5;
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
    
    this->updateThread = std::thread(VibratoNode::update, this); 
    std::cout << "End of vibrato ctor" << std::endl;
}

VibratoNode::~VibratoNode(){
    this->done = true;
    this->updateThread.join();
}

void VibratoNode::update(VibratoNode *self) {
    int period_us = self->format.getPeriodMicroSeconds();

    while(!self->done) {
        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);

        std::vector<sample_type> inData(VIBRATO_CAPACITY);
        inData.clear();
        for (int i = 0; i < self->format.bufferSize; i++) {
            for (int j = 0; j < self->format.channels; j++) {
                if(!self->input->empty()) {
                    inData.push_back(self->input->pop());
                 }
            }
        }

        for (int i = 0; i < inData.size(); i++) {

                sample_type sample;
                if((i < self->depth) || (i + self->depth > inData.size())) {
                    sample = inData.at(i);
                }
                else {
                    sample = inData.at(static_cast<int>(i + self->depth * sin(2 * M_PI * self->frequency / self->format.sampleRate * i)));
                }

                if(!self->output->full()) {
                    self->output->push(sample);
                } 
                
        }
        std::this_thread::sleep_until(sleep);
    }
} 
 
void VibratoNode::onInputChanged(AttrID attr) {
    spdlog::debug("VibratoNode::onInputChanged");
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

void VibratoNode::onStateChanged(std::map<std::string, AudioParameter> newState, void */*arg*/) {
    auto it = newState.find("frequency");
    if(it != newState.end()) {
        AudioParameter frequencyParam = newState["frequency"];
        if(frequencyParam.getType() != AudioParameterType::TYPE_FLOAT) {
            spdlog::warn("VibratoNode::onStateChange did not receive a float parameter");
        } else {
            spdlog::info("VibratoNode::onStateChange changing frequency parameter");
            this->frequency = frequencyParam.getParamFloat();
        }
    } else {
        spdlog::warn("VibratoNode::onStateChange did not find frequency parameter");
    }

    it = newState.find("depth");
    if(it != newState.end()) {
        AudioParameter depthParam = newState["depth"];
        if(depthParam.getType() != AudioParameterType::TYPE_FLOAT) {
            spdlog::warn("VibratoNode::onStateChange did not receive a float parameter");
        } else {
            spdlog::info("VibratoNode::onStateChange changing depth parameter");
            this->depth = depthParam.getParamFloat();
        }
    } else {
        spdlog::warn("VibratoNode::onStateChange did not find depth parameter");
    }

}

extern "C" {
    int build_node(int id, int nextAttrId, Node ** node) {
        *node = new VibratoNode(id, nextAttrId, nextAttrId + 1);
        return nextAttrId + 2;
    }   
}
} // pipeline
