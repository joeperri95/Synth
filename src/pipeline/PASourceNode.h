#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "AudioFormat.h"
#include "AudioQueue.h"
#include "common/AudioDevice.h"

typedef short sample_type;

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
