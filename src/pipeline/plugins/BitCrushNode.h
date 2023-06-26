#pragma once

#include "Node.h"
#include <vector>
#include <thread>
#include <chrono>
#include <map>
#include "common/RingBuffer.h"
#include "common/AudioFormat.h"
#include "common/AudioQueue.h"
#include "common/AudioParameter.h"

namespace pipeline {

class BitCrushNode : public Node {
public:
    BitCrushNode(NodeID id, AttrID inputID, AttrID outputID);
    ~BitCrushNode();
    static void update(BitCrushNode *self);
    void onInputChanged(AttrID attr) override;
    void onStateChanged(std::map<std::string, AudioParameter> newState, void *args);
     
private:
    AttrID inputID;
    AttrID outputID;
    std::shared_ptr<audio::AudioQueue<sample_type>> input;
    std::shared_ptr<audio::AudioQueue<sample_type>> output;
    audio::AudioFormat format;
    bool done;
    std::thread updateThread;

};

extern "C" {
   int build_node(int id, int nextAttrId, Node ** node);
}

}
