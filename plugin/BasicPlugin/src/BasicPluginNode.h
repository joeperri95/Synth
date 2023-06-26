#pragma once

#include "Node.h"
#include "RingBuffer.h"
#include "AudioFormat.h"
#include "AudioQueue.h"
#include <vector>
#include <map>
#include <portaudio.h>

typedef int16_t sample_type;

namespace pipeline {

class BasicPluginNode : public Node {
public:
    BasicPluginNode(NodeID id, AttrID inputID, AttrID outputID);
    ~BasicPluginNode();
    static void update(BasicPluginNode *self);
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

extern "C" {
   int build_node(int id, int nextAttrId, Node * node);
}

}