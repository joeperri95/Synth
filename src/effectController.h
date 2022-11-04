#pragma once

#include <atomic>

struct EffectController {
    EffectController() {
        volume = 0.5;
        tremolo = false;
        tremolo_depth = 20;
        tremolo_freq = 1.0;
        vibrato = false;
        vibrato_freq = 1.0;
        vibrato_depth = 20;
        bitcrush = false;
        flanger = false;
        flanger_depth = 200;
        flanger_freq = 0.5;
        flanger_decay = 0.8;
        average = false;
        average_taps = 21;
        reverb = false;
        reverb_depth = 20;
        reverb_decay = 0.4;
    }

    std::atomic<float> volume;
    std::atomic<bool> tremolo;
    std::atomic<int> tremolo_depth;
    std::atomic<float> tremolo_freq;
    std::atomic<bool> vibrato;
    std::atomic<float> vibrato_freq;
    std::atomic<int> vibrato_depth;
    std::atomic<bool> bitcrush;
    std::atomic<bool> flanger;
    std::atomic<int> flanger_depth;
    std::atomic<float> flanger_freq;
    std::atomic<float> flanger_decay;
    std::atomic<bool> average;
    std::atomic<int> average_taps;
    std::atomic<bool> reverb;
    std::atomic<int> reverb_depth;
    std::atomic<float> reverb_decay;
};

