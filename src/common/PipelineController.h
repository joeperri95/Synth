#pragma once

#include "pipeline/Node.h"
#include "pipeline/NodeFactory.h"
#include "pipeline/Pipeline.h"
#include "spdlog/spdlog.h"
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include "DisplayContext.h"

#include "ui/nodes/NodeWidget.h"
#include "ui/nodes/NodeWidgetFactory.h"

#include "ui/control/ControlWidget.h"
#include "ui/control/ControlWidgetFactory.h"

using pipeline::AttrID;
using pipeline::LinkID;
using pipeline::NodeID;

class PipelineController {
public:
  PipelineController(DisplayContext context);
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

  DisplayContext context;

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
