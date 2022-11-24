#include "Pipeline.h"

#include <iostream>

namespace pipeline {

Pipeline::Pipeline() : factory(), nodes(), links(){
}

Pipeline::~Pipeline() {
}

void Pipeline::addNode(NodeID id, std::string recipe) {
    std::cout << "Pipeline::addNode: " << "NodeID: " << id << " recipe: " << recipe << std::endl;
    this->nodes[id] = factory.createNode(id, recipe);
    for (auto it : this->nodes[id]->getInputs()) {
        this->attributes[it] = id;
        std::cout << "Node: " << id << " has input: " << it << std::endl;
    }
    for (auto it : this->nodes[id]->getOutputs()) {
        this->attributes[it] = id;
        std::cout << "Node: " << id << " has output: " << it << std::endl;
    }
}

void Pipeline::removeNode(NodeID id) {
    std::cout << "Pipeline::removeNode: " << "NodeID: " << id << std::endl;
    auto it = this->nodes.find(id);
    if (it != this->nodes.end()) {
        this->nodes.erase(it);
    }
}

void Pipeline::linkNodes(LinkID id, AttrID l1, AttrID l2) {
    std::cout << "Pipeline::linkNodes: " << "LinkID: " << id << " start: " << l1 << " end: " << l2 << std::endl;
    this->links[id] = std::make_pair(l1, l2);
    this->buffers[id] = std::make_shared<LockingSPSCRingBuffer<sample_type>>(PIPELINE_CAPACITY);
    std::cout << "Pipeline::linkNodes: I should link node: " << this->attributes[l1] << " attr: " << l1 
              << " to node: " << this->attributes[l2] << " attr: " << l2 << std::endl;
    this->nodes[this->attributes[l1]]->setOutput(l1, this->buffers[id]);
    this->nodes[this->attributes[l2]]->setInput(l2, this->buffers[id]);
}

void Pipeline::removeLink(LinkID id) {
    std::cout << "Pipeline::removeLink: " << "LinkID: " << id << std::endl;
    auto it = this->links.find(id);
    if (it != this->links.end()) {
        this->nodes[this->attributes[it->second.first]]->setOutput(it->second.first, nullptr);
        this->nodes[this->attributes[it->second.second]]->setInput(it->second.second, nullptr);
        this->links.erase(it);
    }
}

} // pipeline
