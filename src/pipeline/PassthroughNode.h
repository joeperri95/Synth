#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "common/RingBuffer.h"
#include "common/AudioFormat.h"
#include "common/AudioQueue.h"
#include <portaudio.h>

typedef int16_t sample_type;

namespace pipeline {

class PassthroughNode : public Node {
public:
    PassthroughNode(NodeID id, AttrID inputID, AttrID outputID);
    ~PassthroughNode();
    static void update(PassthroughNode *self);
    void onInputChanged(AttrID attr) override;
     
protected:
    AttrID outputID;
    AttrID inputID;
    std::shared_ptr<audio::AudioQueue<sample_type>> output;
    std::shared_ptr<audio::AudioQueue<sample_type>> input;
    audio::AudioFormat format;
    bool done;
    std::thread updateThread;
};

}