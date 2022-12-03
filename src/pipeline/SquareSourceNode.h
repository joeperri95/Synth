
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

class SquareSourceNode : public Node {
public:
    SquareSourceNode(NodeID id, AttrID outputID);
    ~SquareSourceNode();
    static void update(SquareSourceNode *self);
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

}
