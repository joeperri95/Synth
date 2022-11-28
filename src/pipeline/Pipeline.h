#pragma once

#include "Node.h"
#include "NodeFactory.h"

#include <string>
#include <map>
#include <memory>

const int PIPELINE_CAPACITY = 44100;

namespace pipeline {

class Pipeline {
public:
    Pipeline();
    ~Pipeline();
    void addNode(NodeID id, std::string recipe); // deprecate
    void addNode(NodeID id, std::unique_ptr<Node> node);
    void removeNode(NodeID id);
    void linkNodes(LinkID id, AttrID l1, AttrID l2);
    void removeLink(LinkID id);

private:
    NodeFactory factory;
    std::map<NodeID, std::unique_ptr<Node>> nodes;
    std::map<LinkID, std::pair<AttrID, AttrID>> links;
    std::map<AttrID, NodeID> attributes;
    std::map<LinkID, std::shared_ptr<LockingSPSCRingBuffer<sample_type>>> buffers;
};

}
