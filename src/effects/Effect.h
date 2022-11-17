#pragma once

#include "common/AudioBuffer.h"
#include "sample_type.h"

namespace audio { namespace effects {

class Effect {
public:
    Effect();
    virtual ~Effect() = 0;
    virtual AudioBuffer<sample_type> operator()(AudioBuffer<sample_type> input) = 0;

protected:
};

inline Effect::Effect() {}
inline Effect::~Effect() {}

}}
