#include "TremoloNode.h"
#include "pipeline/Node.h"

namespace pipeline {

TremoloNode::TremoloNode(NodeID id, AttrID inputID, AttrID outputID){
    this->_id = id;
    this->numInputs = 1;
    this->numOutputs = 1;
    this->inputID = inputID;
    this->outputID = outputID;
    this->amplitude = 0.5;
    this->frequency = 2;
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
    
    this->updateThread = std::thread(TremoloNode::update, this); 
}

TremoloNode::~TremoloNode(){
    this->done = true;
    this->updateThread.join();
}

void TremoloNode::update(TremoloNode *self) {
    int period_us = self->format.getPeriodMicroSeconds();

    while(!self->done) {
        auto now = std::chrono::high_resolution_clock::now();
        auto sleep = now + std::chrono::microseconds(period_us);
        for (int i = 0; i < self->format.bufferSize; i++) {
            if(!self->input->empty()) {
                sample_type sample = self->input->pop();
                sample_type out = static_cast<sample_type>(std::sin(2 * M_PI * self->frequency / (1.0 * self->format.sampleRate) * i) * sample);

                if(!self->output->full()) {
                    self->output->push(out);
                } 
            } else {
                //spdlog::debug("TremoloNode::update input queue is empty and isQueueValid == {}", self->input->isQueueValid());
            }
        }
        std::this_thread::sleep_until(sleep);
    }
} 
 
void TremoloNode::onInputChanged(AttrID attr) {
    spdlog::debug("TremoloNode::onInputChanged");
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

void TremoloNode::onStateChanged(std::map<std::string, AudioParameter> newState, void */*arg*/) {
    auto it = newState.find("amplitude");
    if(it != newState.end()) {
        AudioParameter amplitudeParam = newState["amplitude"];
        if(amplitudeParam.getType() != AudioParameterType::TYPE_FLOAT) {
            spdlog::warn("TremoloNode::onStateChange did not receive a float parameter");
        } else {
            spdlog::info("TremoloNode::onStateChange changing amplitude parameter");
            this->amplitude = amplitudeParam.getParamFloat();
        }
    } else {
        spdlog::warn("TremoloNode::onStateChange did not find amplitude parameter");
    }

    it = newState.find("frequency");
    if(it != newState.end()) {
        AudioParameter frequencyParam = newState["frequency"];
        if(frequencyParam.getType() != AudioParameterType::TYPE_FLOAT) {
            spdlog::warn("TremoloNode::onStateChange did not receive a float parameter");
        } else {
            spdlog::info("TremoloNode::onStateChange changing frequency parameter");
            this->frequency = frequencyParam.getParamFloat();
        }
    } else {
        spdlog::warn("TremoloNode::onStateChange did not find frequency parameter");
    }

}

extern "C" {
    int build_node(int id, int nextAttrId, Node ** node) {
        *node = new TremoloNode(id, nextAttrId, nextAttrId + 1);
        return nextAttrId + 2;
    }   
}
} // pipeline
