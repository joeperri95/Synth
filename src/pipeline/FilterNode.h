#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "RingBuffer.h"

namespace pipeline {

template <typename T>
class FilterNode : protected Node {
public:
    FilterNode();
    virtual ~FilterNode() = 0;
    virtual void update() = 0;
     
protected:
    std::map<LinkID, std::shared_ptr<RingBuffer<T>> inputs;
    std::map<LinkID, std::shared_ptr<RingBuffer<T>> outputs;
    unsigned int numInputs;
    unsigned int numOutputs;
};

}