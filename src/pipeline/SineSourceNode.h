#pragma once

#include "Node.h"
#include <vector>
#include <thread>
#include <chrono>
#include <map>
#include "common/RingBuffer.h"
#include "AudioFormat.h"
#include "AudioQueue.h"
#include <cmath>

namespace pipeline {

class SineSourceNode : public Node {
public:
    SineSourceNode(NodeID id, AttrID outputID);
    ~SineSourceNode();
    static void update(SineSourceNode *self);
    void onInputChanged(AttrID attr) override;
     
private:
    AttrID outputID;
    std::shared_ptr<audio::AudioQueue<sample_type>> output;
    audio::AudioFormat format;
    bool done;
    double frequency;
    std::thread updateThread;
};

}
