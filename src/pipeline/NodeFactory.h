#pragma once

#include "Node.h"

#include <string>
#include <memory>

namespace pipeline {

class NodeFactory {
public:
    NodeFactory();
    ~NodeFactory();

    std::unique_ptr<Node> createNode(std::string recipe);

private:
    LinkID nextLinkID;
    NodeID nextNodeID;
};

} // pipeline