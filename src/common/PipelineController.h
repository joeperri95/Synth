#pragma once

#include <string>
#include <map>
#include <memory>
#include <set>
#include <mutex>

#include "pipeline/Pipeline.h"
#include "pipeline/Node.h"
#include "pipeline/NodeFactory.h"

#include "ui/NodeWidget.h"
#include "ui/ControlWidget.h"

#include "ui/ControlWidgetFactory.h"
#include "ui/NodeWidgetFactory.h"

using pipeline::LinkID;
using pipeline::AttrID;
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
    std::vector<std::shared_ptr<ui::ControlWidget>> getWidgets();

  private:
    std::mutex mutex;
  
    // Pipeline section
    pipeline::Pipeline pipeline;

    // ImNode section
    std::map<LinkID, std::pair<AttrID, AttrID>> links;
    std::map<NodeID, std::shared_ptr<ui::NodeWidget>> nodes;
    std::set<NodeID> selectedNodes;

    // Control widget section
    std::map<NodeID, std::shared_ptr<ui::ControlWidget>> widgets;


    // Common
    LinkID nextLinkID;
    NodeID nextNodeID;

    ui::ControlWidgetFactory controlFactory; 
    ui::NodeWidgetFactory nodeWidgetFactory; 
    pipeline::NodeFactory nodeFactory;

};