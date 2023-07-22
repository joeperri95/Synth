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

class BiquadNode : public Node {
public:
    enum class FilterType {
        lowpass,
        highpass,
        bandpass
    };
    BiquadNode(NodeID id, AttrID inputID, AttrID outputID);
    ~BiquadNode();
    static void update(BiquadNode *self);
    void onInputChanged(AttrID attr) override;
    void onStateChanged(std::map<std::string, AudioParameter> newState, void *args) override;

private:
    void updateParameters(float cutoff, uint32_t filter_type);

    AttrID inputID;
    AttrID outputID;
    std::shared_ptr<audio::AudioQueue<sample_type>> input;
    std::shared_ptr<audio::AudioQueue<sample_type>> output;
    audio::AudioFormat format;

    float b0, b1, b2;
    float a1, a2;

    float cutoff;
    FilterType type;

    bool done;
    std::thread updateThread;
};

extern "C" {
   int build_node(int id, int nextAttrId, Node ** node);
}

}
