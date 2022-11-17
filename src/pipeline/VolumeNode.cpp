#include "VolumeNode.h"

namespace pipeline {

VolumeNode::VolumeNode(NodeID id, int inputID, int outputID){
    this->_id = id;
    this->numInputs = 1;
    this->numOutputs = 1;
    this->inputs[inputID] = nullptr;
    this->outputs[outputID] = nullptr;
}

VolumeNode::~VolumeNode(){
}

void VolumeNode::update() {
} 
 
} // pipeline
