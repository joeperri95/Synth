#pragma once

#include "Node.h"

namespace pipeline {

class NodeFactory {
public:
    LinkID getNextLinkID();
};

} // pipeline