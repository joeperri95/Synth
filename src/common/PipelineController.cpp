#include "PipelineController.h"

using pipeline::LinkID;
using pipeline::NodeID;
using pipeline::Node;
using ui::nodes::NodeWidget;
using ui::ControlWidget;

PipelineController::PipelineController() :  mutex(), pipeline(), links(), nodes(), selectedNodes(), 
                                            nextLinkID(1), nextNodeID(1), controlFactory(), 
                                            nodeWidgetFactory(), nodeFactory() {}

PipelineController::~PipelineController() {}

void PipelineController::notified(NodeID id, AudioParameterMap params, void *arg) {
    PipelineController *self = (PipelineController *) arg;
    spdlog::info("ok got here now dispatch params to pipeline");
    self->notify(id, params);
}

void PipelineController::notify(NodeID id, AudioParameterMap params) {
    this->nodeList[id]->onStateChanged(params, nullptr);
}

NodeID PipelineController::addNode(std::string recipe) {
    std::lock_guard lock(this->mutex);
    int ret = nextNodeID++;

    std::shared_ptr<Node> pnode = nodeFactory.createNode(ret, recipe);
    if (pnode != nullptr){
        this->nodeList[ret] = pnode;
        this->pipeline.addNode(ret, pnode);
    }

    std::shared_ptr<NodeWidget> node = nodeWidgetFactory.create(ret, recipe);
    if (node != nullptr) {
        this->nodes[ret] = node;
    }

    std::shared_ptr<ControlWidget> widget = controlFactory.create(ret, recipe);
    if (widget != nullptr) {
        this->widgets[ret] = widget;
        this->widgets[ret]->addSubscriber(ret, PipelineController::notified, this);
    }

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
    spdlog::debug("PipelineController::addLink: created link: {} start: {} end: {}", ret, start, end);
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

std::vector<std::shared_ptr<ui::nodes::NodeWidget>> PipelineController::getNodes() {
    std::lock_guard lock(this->mutex);
    std::vector<std::shared_ptr<ui::nodes::NodeWidget>> ret;
    for (const auto & [key, value]: this->nodes) {
        ret.push_back(value);
    }
    return ret;
}

std::vector<std::shared_ptr<ui::ControlWidget>> PipelineController::getWidgets() {
    std::lock_guard lock(this->mutex);
    std::vector<std::shared_ptr<ui::ControlWidget>> ret;
    for (const auto & [key, value]: this->widgets) {
        ret.push_back(value);
    }
    return ret;
}