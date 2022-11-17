#pragma once

#include <vector>
#include <map>
#include <cstdint>
#include "common/LockingRingBuffer.h"

namespace pipeline {

typedef int LinkID;
typedef int NodeID;
typedef short sample_type;

class Node {
public:
    Node();
    virtual ~Node() = 0;
    virtual void update() = 0;
protected:
    NodeID _id;
    std::map<LinkID, std::shared_ptr<LockingSPSCRingBuffer<sample_type>>> inputs;
    std::map<LinkID, std::shared_ptr<LockingSPSCRingBuffer<sample_type>>> outputs;
    unsigned int numInputs;
    unsigned int numOutputs;
};

inline Node::Node() {}
inline Node::~Node() {}

}