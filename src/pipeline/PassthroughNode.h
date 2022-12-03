#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "RingBuffer.h"
#include "AudioFormat.h"
#include <portaudio.h>

typedef int16_t sample_type;

namespace pipeline {

template <typename T>
class PassthroughNode : protected Node {
public:
    PassthroughNode();
    ~PassthroughNode();
    void update();
     
protected:

};

}