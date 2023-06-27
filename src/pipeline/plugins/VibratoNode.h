#pragma once

#include "Node.h"
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <map>
#include "common/LockingRingBuffer.h"
#include "common/RingBuffer.h"
#include "common/AudioFormat.h"
#include "common/AudioQueue.h"
#include "common/AudioParameter.h"

namespace pipeline {

class VibratoNode : public Node {
public:
    VibratoNode(NodeID id, AttrID inputID, AttrID outputID);
    ~VibratoNode();
    static void update(VibratoNode *self);
    void onInputChanged(AttrID attr) override;
    void onStateChanged(std::map<std::string, AudioParameter> newState, void *args);
     
private:
    AttrID inputID;
    AttrID outputID;
    std::shared_ptr<audio::AudioQueue<sample_type>> input;
    std::shared_ptr<audio::AudioQueue<sample_type>> output;

    audio::AudioFormat format;
    float depth;
    float frequency;
    bool done;
    std::thread updateThread;

};

extern "C" {
   int build_node(int id, int nextAttrId, Node ** node);
}

}
