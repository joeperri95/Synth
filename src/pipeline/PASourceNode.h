#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "common/AudioFormat.h"
#include "common/AudioQueue.h"
#include "common/AudioDevice.h"

typedef int16_t sample_type;

namespace pipeline {

class PASourceNode : public Node {
public:
    PASourceNode(NodeID node, AttrID inputID);
    ~PASourceNode();
    void onInputChanged(AttrID attr);
    void onStateChanged(std::map<std::string, AudioParameter> newState, void *args);
    void update();
     
private:
    audio::AudioFormat format; 
    std::shared_ptr<audio::AudioQueue<sample_type>> queue;
    AttrID outputID;
    StreamID stream;

};

}
