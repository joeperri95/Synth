#pragma once

#include "Node.h"
#include <vector>
#include <map>
#include "RingBuffer.h"
#include "AudioFormat.h"
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define BUFF_SIZE 4096 
#define BIT_DEPTH 8 

typedef short sample_type;

namespace pipeline {

template <typename T>
class PAOutputNode : protected OutputNode {
public:
    PAOutputNode();
    ~PAOutputNode();
    void update();
     
protected:

};

}