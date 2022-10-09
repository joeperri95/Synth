#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "RingBuffer.h"

namespace pipeline {

template <typename T>
class OutputNode : protected Node {
public:
    OutputNode();
    virtual ~OutputNode() = 0;
    virtual void update() = 0;
     
protected:
    std::map<LinkID, std::shared_ptr<RingBuffer<T>> outputs;
    unsigned int numOutputs;
};

}