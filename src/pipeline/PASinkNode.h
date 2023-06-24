#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include <chrono>
#include "common/AudioFormat.h"
#include "common/AudioQueue.h"
#include "common/AudioDevice.h"

typedef short sample_type;

namespace pipeline {

class PASinkNode : public Node {
public:
    PASinkNode(NodeID node, AttrID outputID);
    ~PASinkNode();
    void onInputChanged(AttrID attr);
    void onStateChanged(std::map<std::string, AudioParameter> newState, void *args);
    void update();
     
private:
    audio::AudioFormat format; 
    std::shared_ptr<audio::AudioQueue<sample_type>> queue;
    AttrID inputID;
    StreamID stream;

};

} // pipeline