#include <iostream>
#include <cmath>
#include <array>
#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
#include <chrono>

#include "spdlog/spdlog.h"
#include "common/GUI.h"
#include "wav.h"
#include "common/AudioBuffer.h"
#include "AudioFormat.h"
#include "AudioQueue.h"
#include "common/AudioDevice.h"

using audio::AudioBuffer;
using audio::AudioFormat;
using audio::AudioQueue;

int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::debug);

    AudioFormat format(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    ui::GUI g;
    g.initialize();

    while(!g.shouldQuit) {
        g.handle_event();
        g.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }    

    g.shut_down();

    return 0;
}