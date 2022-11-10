#include "Pipeline.h"

#include <iostream>

namespace pipeline {

void Pipeline::addNode(NodeID id, std::string recipe) {
    std::cout << "Pipeline::addNode: " << "NodeID: " << id << " recipe: " << recipe << std::endl;
}

void Pipeline::removeNode(NodeID id) {
    std::cout << "Pipeline::removeNode: " << "NodeID: " << id << std::endl;
}

void Pipeline::linkNodes(LinkID id, int l1, int l2) {
    std::cout << "Pipeline::linkNodes: " << "LinkID: " << id << " start: " << l1 << " end:" << l2 << std::endl;
}

void Pipeline::removeLink(LinkID id) {
    std::cout << "Pipeline::removeLink: " << "LinkID: " << id << std::endl;
}

} // pipeline