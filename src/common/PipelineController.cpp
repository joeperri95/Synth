#include "PipelineController.h"

using pipeline::LinkID;
using pipeline::NodeID;

PipelineController::PipelineController() : mutex(), pipeline(), links(), nodes(), selectedNodes(), nextNodeID(1), nextLinkID(1), factory(){
}

PipelineController::~PipelineController() {

}

NodeID PipelineController::addNode(std::string recipe) {
    std::lock_guard lock(this->mutex);
    int ret = nextNodeID++;
    this->pipeline.addNode(ret, recipe);
    this->nodes.insert(std::make_pair(ret, factory.create(ret, recipe)));
    return ret;
}

void PipelineController::removeNode(NodeID node) {
    std::lock_guard lock(this->mutex);
    this->pipeline.removeNode(node);
    this->nodes.erase(node);
}

void PipelineController::selectNodes(std::vector<NodeID> nodes) {
    std::lock_guard lock(this->mutex);
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
    std::lock_guard lock(this->mutex);
    int ret = nextLinkID++;
    std::cout << "created link: " << ret << " start: " << start << " end: " << end << std::endl;
    this->links[ret] = std::make_pair(start, end);
    this->pipeline.linkNodes(ret, start, end);

    return ret; 
}

void PipelineController::removeLink(LinkID link) {
    std::lock_guard lock(this->mutex);
    this->links.erase(link);
    this->pipeline.removeLink(link);
}

std::map<LinkID, std::pair<int, int>> PipelineController::getLinks() {
    std::lock_guard lock(this->mutex);
    return this->links;
}

std::vector<std::shared_ptr<ui::NodeWidget>> PipelineController::getNodes() {
    std::lock_guard lock(this->mutex);
    std::vector<std::shared_ptr<ui::NodeWidget>> ret;
    for (const auto & [key, value]: this->nodes) {
        ret.push_back(value);
    }
    return ret;
}
