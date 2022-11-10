#include "PipelineController.h"

using pipeline::LinkID;
using pipeline::NodeID;

PipelineController::PipelineController() : pipeline(), links(), nodes(), selectedNodes(), nextNodeID(0), nextLinkID(0), factory(){
}

PipelineController::~PipelineController() {

}

NodeID PipelineController::addNode(std::string recipe) {
    int ret = nextNodeID;
    this->pipeline.addNode(ret, recipe);
    this->nodes.insert(std::make_pair(ret, factory.create(recipe)));
    return ret;
}

void PipelineController::removeNode(NodeID node) {
    this->pipeline.removeNode(node);
    this->nodes.erase(node);
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
    this->selectedNodes = newSelected;
}

LinkID PipelineController::addLink(int start, int end) {
    int ret = nextLinkID;
    this->links[nextLinkID++] = std::make_pair(start, end);
    this->pipeline.linkNodes(ret, start, end);

    return ret; 
}

void PipelineController::removeLink(LinkID link) {
    this->links.erase(link);
}

std::vector<std::pair<int, int>> PipelineController::getLinks() {
    std::vector<std::pair<int, int>> ret;
    for (auto it: this->links) {
        ret.push_back(it.second);
    }
    return ret;
}

std::vector<std::shared_ptr<ui::NodeWidget>> PipelineController::getNodes() {
    std::vector<std::shared_ptr<ui::NodeWidget>> ret;
    for (auto it: this->nodes) {
        ret.push_back(it.second);
    }
    return ret;
}
