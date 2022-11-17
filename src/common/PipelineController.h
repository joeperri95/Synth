#pragma once

#include <string>
#include <map>
#include <memory>
#include <set>
#include <mutex>

#include "pipeline/Pipeline.h"
#include "pipeline/Node.h"
#include "ui/NodeWidget.h"

#include "ui/NodeWidgetFactory.h"

using pipeline::LinkID;
using pipeline::NodeID;

class PipelineController {
  public:
    PipelineController();
    ~PipelineController();

    NodeID addNode(std::string recipe);
    void removeNode(NodeID node);

    void selectNodes(std::vector<NodeID> nodes);

    LinkID addLink(int start, int end);
    void removeLink(LinkID link);

    std::map<LinkID, std::pair<int, int>> getLinks();
    std::vector<std::shared_ptr<ui::NodeWidget>> getNodes();

  private:
    std::mutex mutex;
  
    // Pipeline section
    pipeline::Pipeline pipeline;

    // ImNode section
    std::map<LinkID, std::pair<int, int>> links;
    std::map<NodeID, std::shared_ptr<ui::NodeWidget>> nodes;
    std::set<NodeID> selectedNodes;

    LinkID nextLinkID;
    NodeID nextNodeID;

    ui::NodeWidgetFactory factory; 

};