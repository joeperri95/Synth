#pragma once

#include "Node.h"
#include <vector>
#include <thread>
#include <chrono>
#include <map>
#include "common/RingBuffer.h"
#include "AudioFormat.h"
#include "AudioQueue.h"
#include "AudioParameter.h"

namespace pipeline {

class TremoloNode : public Node {
public:
    TremoloNode(NodeID id, AttrID inputID, AttrID outputID);
    ~TremoloNode();
    static void update(TremoloNode *self);
    void onInputChanged(AttrID attr) override;
    void onStateChanged(std::map<std::string, AudioParameter> newState, void *args);
     
private:
    AttrID inputID;
    AttrID outputID;
    std::shared_ptr<audio::AudioQueue<sample_type>> input;
    std::shared_ptr<audio::AudioQueue<sample_type>> output;
    audio::AudioFormat format;
    float amplitude;
    float frequency;
    bool done;
    std::thread updateThread;

};

}
