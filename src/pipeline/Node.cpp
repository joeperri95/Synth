#include "Node.h"
#include <iostream>

namespace pipeline {

Node::Node() {}
Node::~Node() {}

std::vector<AttrID> Node::getInputs() {
    std::vector<AttrID> ret;
    for (auto it: this->inputs) {
        ret.push_back(it.first);
    }
    return ret;
}

std::vector<AttrID> Node::getOutputs() {
    std::vector<AttrID> ret;
    for (auto it: this->outputs) {
        ret.push_back(it.first);
    }
    return ret;
}

void Node::onStateChanged(std::map<std::string, AudioParameter> /*newState*/, void */*args*/) {}

void Node::setInput(AttrID attr, std::shared_ptr<LockingSPSCRingBuffer<sample_type>> input) {
    if (this->inputs.find(attr) != this->inputs.end()) {
        if (this->inputs[attr]) {
            this->inputs[attr].swap(input);
        } else {
            this->inputs[attr] = input;
        }
        this->onInputChanged(attr);
    } else {
        std::cerr << "Attr " << attr << " not in input map!" << std::endl;
    }
}

void Node::setOutput(AttrID attr, std::shared_ptr<LockingSPSCRingBuffer<sample_type>> output) {
    if (this->outputs.find(attr) != this->outputs.end()) {
        if (this->outputs[attr]) {
            this->outputs[attr].swap(output);
        } else {
            this->outputs[attr] = output;
        }
        this->onInputChanged(attr);
    } else {
        std::cerr << "Attr " << attr << " not in output map!" << std::endl;
    }
}

}