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
    std::shared_ptr<Node> loadNode(NodeID id, std::string recipe);
private:
    void loadRecord(std::string filename); 
    int nextAttrID;
    std::map<std::string, std::string> recipes;

};

} // pipeline