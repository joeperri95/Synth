#pragma once

#include "Node.h"

namespace pipeline {

class Pipeline {
public:

    void addNode(Node n);
    void linkNodes(LinkID ll, LinkID l2);
    void removeLink(LinkID l1);

private:
    std::vector<Node> nodes;
    std::vector<std::pair<LinkID, LinkID>> links;
};

}