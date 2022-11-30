#include "Pipeline.h"

#include "spdlog/spdlog.h"

namespace pipeline {

Pipeline::Pipeline() : factory(), nodes(), links(){
}

Pipeline::~Pipeline() {
}

void Pipeline::addNode(NodeID id, std::string recipe) {
    spdlog::debug("Pipeline::addNode: NodeID: {} recipe: {}", id, recipe);
    this->nodes[id] = factory.createNode(id, recipe);
    for (auto it : this->nodes[id]->getInputs()) {
        this->attributes[it] = id;
        spdlog::debug("Node: {} has input: {}", id, it);
    }
    for (auto it : this->nodes[id]->getOutputs()) {
        this->attributes[it] = id;
        spdlog::debug("Node: {} has output: {}", id, it);
    }
}

void Pipeline::addNode(NodeID id, std::shared_ptr<Node> node) {
    spdlog::debug("Pipeline::addNode: NodeID: {}", id);
    this->nodes[id] = std::move(node);
    for (auto it : this->nodes[id]->getInputs()) {
        this->attributes[it] = id;
        spdlog::debug("Node: {} has input: {}", id, it);
    }
    for (auto it : this->nodes[id]->getOutputs()) {
        this->attributes[it] = id;
        spdlog::debug("Node: {} has output: {}", id, it);
    }
}


void Pipeline::removeNode(NodeID id) {
    spdlog::debug("Pipeline::removeNode: NodeID: {}", id);
    auto it = this->nodes.find(id);
    if (it != this->nodes.end()) {
        this->nodes.erase(it);
    }
}

void Pipeline::linkNodes(LinkID id, AttrID l1, AttrID l2) {
    spdlog::debug("Pipeline::linkNodes: LinkID: {} start: {} end: {}", id, l1, l2);
    this->links[id] = std::make_pair(l1, l2);
    this->buffers[id] = std::make_shared<LockingSPSCRingBuffer<sample_type>>(PIPELINE_CAPACITY);
    spdlog::debug("Pipeline::linkNodes: linking node: {} attr: {} to node: {} attr: {}",
                   this->attributes[l1], l1, this->attributes[l2], l2);
    this->nodes[this->attributes[l1]]->setOutput(l1, this->buffers[id]);
    this->nodes[this->attributes[l2]]->setInput(l2, this->buffers[id]);
}

void Pipeline::removeLink(LinkID id) {
    spdlog::debug("Pipeline::removeLink: {}", id);
    auto it = this->links.find(id);
    if (it != this->links.end()) {
        this->nodes[this->attributes[it->second.first]]->setOutput(it->second.first, nullptr);
        this->nodes[this->attributes[it->second.second]]->setInput(it->second.second, nullptr);
        this->links.erase(it);
    }
}

} // pipeline
