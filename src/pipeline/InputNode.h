#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "RingBuffer.h"

namespace pipeline {

template <typename T>
class InputNode : protected Node {
public:
    InputNode();
    virtual ~InputNode() = 0;
    virtual void update() = 0;
     
protected:
    std::map<LinkID, std::shared_ptr<RingBuffer<T>> inputs;
    unsigned int numInputs;
};

}