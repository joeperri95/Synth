#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "common/RingBuffer.h"
#include "AudioFormat.h"

namespace pipeline {

class VolumeNode : public Node {
public:
    VolumeNode(NodeID id, AttrID inputID, AttrID outputID);
    ~VolumeNode();
    void update() override;
     
private:
    AttrID inputID;
    AttrID outputID;
    audio::AudioFormat format;

};

}
