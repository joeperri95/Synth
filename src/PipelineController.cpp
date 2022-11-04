#include "PipelineController.h"

using pipeline::LinkID;
using pipeline::NodeID;

PipelineController::PipelineController() {

}

PipelineController::~PipelineController() {

}

NodeID PipelineController::addNode(std::string recipe) {
    this->pipeline.addNode(recipe);
}

void PipelineController::removeNode(NodeID node) {

}

void PipelineController::selectNodes(std::vector<NodeID> nodes) {
    std::set<NodeID> newSelected;
    for (auto node: nodes) {
        if (this->selectedNodes.contains(node)) {
            newSelected.insert(node);
        }
        else {
            this->nodes[node]->onSelected();
            newSelected.insert(node);
        }
    }
}

LinkID PipelineController::addLink(int start, int end) {
    this->links[nextLinkID++] = std::make_pair(start,end);
    this->pipeline.linkNodes();
}

void PipelineController::removeLink(LinkID link) {
    this->links.erase(link);
}

std::vector<std::pair<int, int>> PipelineController::getLinks() {
    std::vector<std::pair<int, int>> ret;
    for (auto it: this->links) {
        ret->push_back(it);
    }
    return ret;
}

std::vector<std::shared_ptr<ui::NodeWidget>> PipelineController::getNodes() {
    std::vector<std::pair<int, int>> ret;
    for (auto it: this->nodes) {
        ret->push_back(it);
    }
    return ret;
}
