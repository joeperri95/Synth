#pragma once

#include "Node.h"
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <map>
#include "common/RingBuffer.h"
#include "common/AudioFormat.h"
#include "common/AudioQueue.h"
#include "common/AudioParameter.h"

namespace pipeline {

class MixerNode : public Node {
public:
    MixerNode(NodeID id, AttrID input1ID, AttrID input2ID, AttrID outputID);
    ~MixerNode();
    static void update(MixerNode *self);
    void onInputChanged(AttrID attr) override;
     
private:
    AttrID input1ID;
    AttrID input2ID;
    AttrID outputID;
    std::map<AttrID, std::shared_ptr<audio::AudioQueue<sample_type>>> inputQueues; 
    std::shared_ptr<audio::AudioQueue<sample_type>> output;
    audio::AudioFormat format;
    bool done;
    std::thread updateThread;

};

extern "C" {
   int build_node(int id, int nextAttrId, Node ** node);
}

}
