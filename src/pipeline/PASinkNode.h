#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "AudioFormat.h"
#include "AudioQueue.h"
#include "common/AudioDevice.h"

typedef short sample_type;

namespace pipeline {

class PASinkNode : public Node {
public:
    PASinkNode(NodeID node, AttrID outputID);
    ~PASinkNode();
    void onInputChanged(AttrID attr);
    void update();
     
private:
    audio::AudioFormat format; 
    std::shared_ptr<audio::AudioQueue<sample_type>> queue;
    AttrID inputID;
    StreamID stream;

};

} // pipeline