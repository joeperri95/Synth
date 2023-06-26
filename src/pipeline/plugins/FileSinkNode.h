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

class FileSinkNode : public Node {
public:
    FileSinkNode(NodeID id, AttrID inputID);
    ~FileSinkNode();
    static void update(FileSinkNode *self);
    void onInputChanged(AttrID attr) override;
    void onStateChanged(std::map<std::string, AudioParameter> newState, void *args);
     
private:
    AttrID inputID;
    std::shared_ptr<audio::AudioQueue<sample_type>> queue;
    audio::AudioFormat format;
    bool done;
    bool validQueue;
    std::thread updateThread;

};

extern "C" {
   int build_node(int id, int nextAttrId, Node ** node);
}

}
