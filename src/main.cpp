#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "common/AudioBuffer.h"
#include "common/AudioDevice.h"
#include "common/AudioFormat.h"
#include "common/AudioQueue.h"
#include "common/GUI.h"
#include "spdlog/spdlog.h"
#include "wav.h"

using audio::AudioFormat;

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::debug);
  AudioFormat format(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

  ui::GUI g;
  g.initialize();

  while (!g.shouldQuit) {
    g.handle_event();
    g.render();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  g.shut_down();

  return 0;
}
