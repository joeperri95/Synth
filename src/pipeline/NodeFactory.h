#pragma once

#include "Node.h"

#include <string>
#include <memory>
#include "spdlog/spdlog.h"

namespace pipeline {

class NodeFactory {
public:
    NodeFactory();
    ~NodeFactory();

    std::shared_ptr<Node> createNode(NodeID id, std::string recipe);
private:
    int nextAttrID;

};

} // pipeline