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

class SplitterNode : public Node {
public:
    SplitterNode(NodeID id, AttrID inputID, AttrID output1ID, AttrID output2ID);
    ~SplitterNode();
    static void update(SplitterNode *self);
    void onInputChanged(AttrID attr) override;
     
private:
    AttrID inputID;
    AttrID output1ID;
    AttrID output2ID;
    std::map<AttrID, std::shared_ptr<audio::AudioQueue<sample_type>>> outputQueues; 
    std::shared_ptr<audio::AudioQueue<sample_type>> input;
    audio::AudioFormat format;
    bool done;
    std::thread updateThread;

};

extern "C" {
   int build_node(int id, int nextAttrId, Node ** node);
}

}
