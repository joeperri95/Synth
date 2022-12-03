#pragma once

#include "Node.h"
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <map>
#include "common/RingBuffer.h"
#include "AudioFormat.h"
#include "AudioQueue.h"
#include "AudioParameter.h"

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

}
