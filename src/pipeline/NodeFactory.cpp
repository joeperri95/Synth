#include "NodeFactory.h"

#include "nlohmann/json.hpp"
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <fstream>

#include "PASinkNode.h"
#include "PASourceNode.h"

using json = nlohmann::json;

namespace pipeline {

NodeFactory::NodeFactory() : nextAttrID(1) {loadRecord("etc/recipes.json");}
NodeFactory::~NodeFactory() {}

std::shared_ptr<Node> NodeFactory::createNode(NodeID id, std::string recipe) {
    // exclude portaudio nodes from the plugin framework for now
    if(recipes.contains(recipe)) {
        if(recipe.compare("microphone") == 0)  {
            std::shared_ptr<Node> ret = std::unique_ptr<Node>(new PASourceNode(id, nextAttrID++));
            return ret;
        } else if(recipe.compare("sink") == 0)  {
            spdlog::info("Creating sink");
            std::shared_ptr<Node> ret = std::unique_ptr<Node>(new PASinkNode(id, nextAttrID++));
            return ret;
        } else {
            return loadNode(id, recipe);
        }
    } else {
        spdlog::warn("Pipeline recipe not found {}", recipe);
        return nullptr;
    }
}

std::shared_ptr<Node> NodeFactory::loadNode(NodeID id, std::string recipe) { 
    std::string filename(recipes[recipe]);

    void * handle = dlopen(filename.c_str(), RTLD_LAZY);
    if (handle == nullptr) {
        spdlog::error("Issue opening library {} {}", filename, dlerror());
        exit(1);   
    }

    void * function_return = dlsym(handle, "build_node"); 

    if (function_return == nullptr) {
        spdlog::error("Issue loading symbol build_node {}", dlerror());
        exit(1);   
    }

    int (*function)(int, int, Node **) = reinterpret_cast<int (*)(int, int, Node **)>(function_return);

    if (function == nullptr) {
        spdlog::error("Issue forming function from symbol build_node {}", dlerror());
        exit(1);   
    }

    Node *node = nullptr;
    nextAttrID = function(id, nextAttrID, &node);
    
    if(node == nullptr) {
        spdlog::error("Issue creating node in library file");
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
