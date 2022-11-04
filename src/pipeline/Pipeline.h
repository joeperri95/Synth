#pragma once

#include "Node.h"

namespace pipeline {

class Pipeline {
public:
    void addNode(NodeID id, std::string recipe);
    void removeNode(NodeID id);
    void linkNodes(LinkID id, int l1, int l2);
    void removeLink(LinkID id);

private:
    std::map<NodeID, std::unique_ptr<Node>> nodes;
    std::map<LinkID, std::pair<int, int>> links;
};

}