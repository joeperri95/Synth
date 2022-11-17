#pragma once

#include "Node.h"

#include <string>
#include <memory>

namespace pipeline {

class NodeFactory {
public:
    NodeFactory();
    ~NodeFactory();

    std::unique_ptr<Node> createNode(NodeID id, std::string recipe);
private:
    int nextAttrID;

};

} // pipeline