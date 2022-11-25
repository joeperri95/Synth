#pragma once

#include "Node.h"
#include <vector>
#include <thread>
#include <chrono>
#include <map>
#include "common/RingBuffer.h"
#include "AudioFormat.h"
#include "AudioQueue.h"

namespace pipeline {

class VolumeNode : public Node {
public:
    VolumeNode(NodeID id, AttrID inputID, AttrID outputID);
    ~VolumeNode();
    static void update(VolumeNode *self);
    void onInputChanged(AttrID attr) override;
     
private:
    AttrID inputID;
    AttrID outputID;
    std::shared_ptr<audio::AudioQueue<sample_type>> input;
    std::shared_ptr<audio::AudioQueue<sample_type>> output;
    audio::AudioFormat format;
    float volume;
    bool done;
    std::thread updateThread;

};

}
