#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "RingBuffer.h"
#include "AudioFormat.h"
#include <portaudio.h>

typedef short sample_type;

namespace pipeline {

template <typename T>
class PassthroughNode : protected FilterNode {
public:
    PassthroughNode();
    ~PassthroughNode();
    void update();
     
protected:

};

}