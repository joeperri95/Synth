#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "common/RingBuffer.h"

namespace pipeline {

class VolumeNode : public Node {
public:
    VolumeNode(NodeID id, int inputID, int outputID);
    ~VolumeNode();
    void update() override;
     
private:

};

}
