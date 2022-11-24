#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "AudioFormat.h"
#include "AudioQueue.h"
#include "common/AudioDevice.h"

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define BUFF_SIZE 4096 
#define BIT_DEPTH 8 

typedef short sample_type;

namespace pipeline {

class PASourceNode : public Node {
public:
    PASourceNode(NodeID node, AttrID inputID);
    ~PASourceNode();
    void update();
     
private:
    audio::AudioFormat format; 
    std::shared_ptr<audio::AudioQueue<sample_type>> queue;
    AttrID outputID;
    StreamID stream;

};

}
