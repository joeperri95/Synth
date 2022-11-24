#include "NodeFactory.h"
#include "VolumeNode.h"
#include "PASourceNode.h"
#include "PASinkNode.h"

#include <iostream>
#include <memory>

namespace pipeline {

NodeFactory::NodeFactory() : nextAttrID(1) {}
NodeFactory::~NodeFactory() {}

std::unique_ptr<Node> NodeFactory::createNode(NodeID id, std::string recipe) {
    if (recipe.compare("volume") == 0) {
        std::unique_ptr<Node> ret = std::unique_ptr<Node>(new VolumeNode(id, nextAttrID, nextAttrID + 1));
        nextAttrID += 2;
        return ret;
    }
    else if (recipe.compare("source") == 0) {
        std::unique_ptr<Node> ret = std::unique_ptr<Node>(new PASourceNode(id, nextAttrID));
        nextAttrID += 1;
        return ret;
    }
    else if (recipe.compare("sink") == 0) {
        std::unique_ptr<Node> ret = std::unique_ptr<Node>(new PASinkNode(id, nextAttrID));
        nextAttrID += 1;
        return ret;
    }
    else {
        std::cout << "Recipe not found" << std::endl;
        return nullptr;
    }
}



} // pipeline
