#pragma once

#include "Node.h"
#include <vector>
#include <thread>
#include <chrono>
#include <map>
#include "common/RingBuffer.h"
#include "common/AudioFormat.h"
#include "common/AudioQueue.h"
#include <cmath>

namespace pipeline {

class SineSourceNode : public Node {
public:
    SineSourceNode(NodeID id, AttrID outputID);
    ~SineSourceNode();
    static void update(SineSourceNode *self);
    void onInputChanged(AttrID attr) override;
    void onStateChanged(std::map<std::string, AudioParameter> newState, void *args);
     
private:
    AttrID outputID;
    std::shared_ptr<audio::AudioQueue<sample_type>> output;
    audio::AudioFormat format;
    bool done;
    float frequency;
    std::thread updateThread;
};

extern "C" {
   int build_node(int id, int nextAttrId, Node ** node);
}

}
