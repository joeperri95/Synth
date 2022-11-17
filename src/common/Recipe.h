#pragma once

#include <string>
#include <vector>

#include "AttributeInfo.h"
#include "pipeline/Node.h"

// Metadata needed by a factory to build a node object 

struct Recipe {
    std::string name;
    pipeline::NodeID nodeID;
    std::vector<Attribute> attributes;
};