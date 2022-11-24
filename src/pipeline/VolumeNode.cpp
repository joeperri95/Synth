#include "VolumeNode.h"

namespace pipeline {

VolumeNode::VolumeNode(NodeID id, AttrID inputID, AttrID outputID){
    this->_id = id;
    this->numInputs = 1;
    this->numOutputs = 1;
    this->inputID = inputID;
    this->outputID = outputID;
    this->inputs[inputID] = nullptr;
    this->outputs[outputID] = nullptr;
}

VolumeNode::~VolumeNode(){
}

void VolumeNode::update() {
} 
 
} // pipeline
