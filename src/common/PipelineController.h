#pragma once

#include <string>
#include <map>
#include <memory>
#include <set>
#include <mutex>
#include "spdlog/spdlog.h"
#include "pipeline/Pipeline.h"
#include "pipeline/Node.h"
#include "pipeline/NodeFactory.h"

#include "ui/nodes/NodeWidget.h"
#include "ui/nodes/NodeWidgetFactory.h"

#include "ui/control/ControlWidget.h"
#include "ui/control/ControlWidgetFactory.h"

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
    std::vector<std::shared_ptr<ui::nodes::NodeWidget>> getNodes();
    std::vector<std::shared_ptr<ui::ControlWidget>> getWidgets();

    // broker params to node
    static void notified(NodeID id, AudioParameterMap params, void *arg);
    void notify(NodeID id, AudioParameterMap params);

  private:
    std::mutex mutex;
  
    // Pipeline section
    pipeline::Pipeline pipeline;
    std::map<NodeID, std::shared_ptr<pipeline::Node>> nodeList;

    // ImNode section
    std::map<LinkID, std::pair<AttrID, AttrID>> links;
    std::map<NodeID, std::shared_ptr<ui::nodes::NodeWidget>> nodes;
    std::set<NodeID> selectedNodes;

    // Control widget section
    std::map<NodeID, std::shared_ptr<ui::ControlWidget>> widgets;


    // Common
    LinkID nextLinkID;
    NodeID nextNodeID;

    ui::ControlWidgetFactory controlFactory; 
    ui::nodes::NodeWidgetFactory nodeWidgetFactory; 
    pipeline::NodeFactory nodeFactory;

};