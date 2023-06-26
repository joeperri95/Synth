#include "NodeFactory.h"
#include "VolumeNode.h"
#include "PASourceNode.h"
#include "PASinkNode.h"
#include "SineSourceNode.h"
#include "SquareSourceNode.h"
#include "MixerNode.h"
#include "PassthroughNode.h"
#include "TremoloNode.h"

#include "nlohmann/json.hpp"
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <fstream>

using json = nlohmann::json;

namespace pipeline {

NodeFactory::NodeFactory() : nextAttrID(1) {loadRecord("etc/recipes.json");}
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
        return loadNode(id, "wav");
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

std::shared_ptr<Node> NodeFactory::loadNode(NodeID id, std::string recipe) { 
    
    std::string filename(recipes[recipe]);

    std::cout << filename << std::endl;
    void * handle = dlopen(filename.c_str(), RTLD_LAZY);
    if (handle == nullptr) {
        std::cout << "Pack it up boys 1 " << dlerror() <<std::endl;
        exit(1);   
    }

    void * function_return = dlsym(handle, "build_node"); 

    if (function_return == nullptr) {
        std::cout << "Pack it up boys 2 " << dlerror() <<std::endl;
        exit(1);   
    }

    int (*function)(int, int, Node **) = reinterpret_cast<int (*)(int, int, Node **)>(function_return);

    if (function == nullptr) {
        std::cout << "Pack it up boys 3 " << dlerror() <<std::endl;
        exit(1);   
    }

    Node *node = nullptr;
    nextAttrID = function(id, nextAttrID, &node);
    
    if(node == nullptr) {
        std::cout << "Pack it up boys 4" <<std::endl;
        exit(1);   
    }
     
    std::shared_ptr<Node> ret = std::unique_ptr<Node>(node);
    return ret;
}

void NodeFactory::loadRecord(std::string filename) {
    std::ifstream ifs(filename);

    json j = json::parse(ifs);    
    json recipe_list = j["recipes"];    
    
    for (const auto &i: recipe_list) {
        std::string title(i["title"]);
        std::string library(i["node_library"]);
        recipes[title] = library;
    }
}


} // pipeline
