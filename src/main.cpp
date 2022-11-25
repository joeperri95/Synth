#define SAMPLE_RATE 44100
#define CHANNELS 2
#define BUFF_SIZE 4096 
#define BIT_DEPTH 8 

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

#include "ui/VolumeWidget.h"
#include "effects/Volume.h"
#include "effects/Effect.h"

#include <portaudio.h>
#include "common/GUI.h"
#include "wav.h"
#include "effectController.h"
#include "common/AudioBuffer.h"
#include "AudioFormat.h"
#include "AudioQueue.h"
#include "common/AudioDevice.h"
#include "effects.h"

using audio::AudioBuffer;
using audio::AudioFormat;
using audio::AudioQueue;

int play_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData);

int record_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData);

void effect_thread(AudioQueue<sample_type> *in, AudioQueue<sample_type> *out, std::shared_ptr<EffectController>, std::shared_ptr<float> f); 
void splitter(AudioQueue<sample_type> *input, AudioQueue<sample_type> *output1, AudioQueue<sample_type> *output2);

int main(int argc, char *argv[])
{
    AudioFormat format(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);

    AudioQueue<sample_type> display;
    display.setFormat({CHANNELS, SAMPLE_RATE,BIT_DEPTH, BUFF_SIZE});

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

void splitter(AudioQueue<sample_type> *input, AudioQueue<sample_type> *output1, AudioQueue<sample_type> *output2)
{    
    while(1) {
        if(!input->empty()) {
            sample_type sample = input->pop();
            output1->push(sample);
            output2->push(sample);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void effect_thread(AudioQueue<sample_type> *in, AudioQueue<sample_type> *out, std::shared_ptr<EffectController> controller, std::shared_ptr<float> f) {
    audio::effects::Volume vol(f);

    while (1) {
        if (!in->empty()) {
            sample_type last = in->pop();

            /* DEPRECATED 
            sample_type next;

            next = audio::effects::volume(last, 0.5);
            last = next;

            if(controller->tremolo) {
               next = audio::effects::tremolo(last, controller->tremolo_freq);
               last = next;
            }
            if (controller->vibrato) {
                next= audio::effects::vibrato(last, controller->vibrato_freq, controller->vibrato_depth);
                last = next;
            }
            if (controller->reverb) {
                next = audio::effects::reverb(last, controller->reverb_depth, controller->reverb_decay);
                last = next;
            }
            if (controller->flanger) {
                next = audio::effects::flanger(last, controller->flanger_depth, controller->flanger_decay, controller->flanger_freq);
                last = next;
            }
            if (controller->bitcrush) {
                next = audio::effects::bitcrush(last);
                last = next;
            }
            if (controller->average) {
                next = audio::effects::average_filter(last, controller->average_taps);
                last = next;
            }

            next = vol(last);

            */ 

            out->push(last);

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}