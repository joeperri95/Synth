#pragma once

#include <vector>
#include <map>
#include <cstdint>
#include "common/LockingRingBuffer.h"

namespace pipeline {

typedef int LinkID;
typedef int AttrID;
typedef int NodeID;
typedef short sample_type;

class Node {
public:
    Node();
    virtual ~Node() = 0;
    virtual void onInputChanged(AttrID attr) = 0;

    std::vector<LinkID> getInputs();
    std::vector<LinkID> getOutputs();

    void setInput(AttrID attr, std::shared_ptr<LockingSPSCRingBuffer<sample_type>> input);
    void setOutput(AttrID attr, std::shared_ptr<LockingSPSCRingBuffer<sample_type>> output);

protected:
    NodeID _id;
    std::map<AttrID, std::shared_ptr<LockingSPSCRingBuffer<sample_type>>> inputs;
    std::map<AttrID, std::shared_ptr<LockingSPSCRingBuffer<sample_type>>> outputs;
    unsigned int numInputs;
    unsigned int numOutputs;
};

}