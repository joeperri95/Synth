#pragma once

#include "Effect.h"
#include <memory>

namespace audio { namespace effects {
class Volume : Effect {
public:
    Volume(std::shared_ptr<float> _volume);
    ~Volume();
    AudioBuffer<sample_type> operator()(AudioBuffer<sample_type> input);

protected:
    std::shared_ptr<float> _volume;
};

}}