#include "NodeFactory.h"
#include "VolumeNode.h"
#include "PASourceNode.h"
#include "PASinkNode.h"
#include "FileSinkNode.h"
#include "FileSourceNode.h"
#include "SineSourceNode.h"
#include "SquareSourceNode.h"
#include "MixerNode.h"
#include "PassthroughNode.h"
#include "TremoloNode.h"

#include <iostream>
#include <memory>

namespace pipeline {

NodeFactory::NodeFactory() : nextAttrID(1) {}
NodeFactory::~NodeFactory() {}

std::shared_ptr<Node> NodeFactory::createNode(NodeID id, std::string recipe) {
    if (recipe.compare("volume") == 0) {
        std::shared_ptr<Node> ret = std::unique_ptr<Node>(new VolumeNode(id, nextAttrID, nextAttrID + 1));
        nextAttrID += 2;
        return ret;
    } else if (recipe.compare("microphone") == 0) {
        std::shared_ptr<Node> ret = std::unique_ptr<Node>(new PASourceNode(id, nextAttrID));
        nextAttrID += 1;
        return ret;
    } else if (recipe.compare("sine") == 0) {
        std::shared_ptr<Node> ret = std::unique_ptr<Node>(new SineSourceNode(id, nextAttrID));
        //std::shared_ptr<Node> ret = std::unique_ptr<Node>(new SquareSourceNode(id, nextAttrID));
        nextAttrID += 1;
        return ret;
    } else if (recipe.compare("sink") == 0) {
        std::shared_ptr<Node> ret = std::unique_ptr<Node>(new PASinkNode(id, nextAttrID));
        //std::shared_ptr<Node> ret = std::unique_ptr<Node>(new FileSinkNode(id, nextAttrID));
        nextAttrID += 1;
        return ret;
    } else if (recipe.compare("mixer") == 0) {
        std::shared_ptr<Node> ret = std::unique_ptr<Node>(new MixerNode(id, nextAttrID, nextAttrID + 1, nextAttrID + 2));
        nextAttrID += 3;
        return ret;
    } else if (recipe.compare("wav") == 0) {
        std::shared_ptr<Node> ret = std::unique_ptr<Node>(new FileSourceNode(id, nextAttrID));
        nextAttrID += 1;
        return ret;
    } else if (recipe.compare("passthrough") == 0) {
        std::shared_ptr<Node> ret = std::unique_ptr<Node>(new PassthroughNode(id, nextAttrID, nextAttrID + 1));
        nextAttrID += 2;
        return ret;
    } else if (recipe.compare("tremolo") == 0) {
        std::shared_ptr<Node> ret = std::unique_ptr<Node>(new TremoloNode(id, nextAttrID, nextAttrID + 1));
        nextAttrID += 2;
        return ret;
    } else {
        spdlog::warn("Pipeline recipe not found {}", recipe);
        return nullptr;
    }
}



} // pipeline
