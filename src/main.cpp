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
    std::cout << "Device initializing " << std::endl;

    AudioFormat format(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);
    AudioQueue<sample_type> audio_in, audio_out, effects, display;
    audio_in.format = format;
    audio_out.format = format;
    effects.format = format;
    display.format = format;

    std::shared_ptr<EffectController> controller= std::make_shared<EffectController>();
    std::shared_ptr<float> volume = std::make_shared<float>(0.5);

    PaStream *stream;
    PaStream *record_stream;

    std::cout << format.channels << " " << format.sampleRate << " " << format.bufferSize << std::endl;
    //build a track with channels
    //build a callback
    
    std::thread t(effect_thread, &audio_in, &effects, controller, volume);
    std::thread t2(splitter, &effects, &audio_out, &display);

    Pa_Initialize();
    Pa_OpenDefaultStream(&stream, 0, format.channels, paInt16, format.sampleRate, format.bufferSize, play_callback, (void *) &audio_out);
    Pa_OpenDefaultStream(&record_stream, format.channels, 0, paInt16, format.sampleRate, format.bufferSize, record_callback, (void *) &audio_in);

    Pa_StartStream(record_stream);
    Pa_StartStream(stream);

    ui::GUI g;
    std::shared_ptr<ui::VolumeWidget> v = make_shared<ui::VolumeWidget>(volume);
    g.initialize(controller, &display, v);

    while(!g.shouldQuit) {
        g.handle_event();
        g.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }    

    g.shut_down();

    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}

void splitter(AudioQueue<sample_type> *input, AudioQueue<sample_type> *output1, AudioQueue<sample_type> *output2)
{    
    while(1) {
        if(!input->_queue.empty()) {
            AudioBuffer<sample_type> sample = input->_queue.front();
            input->_queue.pop_front();
            output1->_queue.push_back(sample);
            output2->_queue.push_back(sample);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void effect_thread(AudioQueue<sample_type> *in, AudioQueue<sample_type> *out, std::shared_ptr<EffectController> controller, std::shared_ptr<float> f) {
    audio::effects::Volume vol(f);

    while (1) {
        if (!in->_queue.empty()) {
            AudioBuffer last = in->_queue.front();
            AudioBuffer<sample_type> next(last.getFormat());

            in->_queue.pop_front();

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
            out->_queue.push_back(next);

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int play_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {

    AudioQueue<sample_type> *queue = (AudioQueue<sample_type> *)userData;

    if (queue->_queue.empty()) {
        return paContinue;
    }

    AudioFormat format = queue->format;
    AudioBuffer buffer = queue->_queue.front();
    queue->_queue.pop_front();
    std::vector<sample_type> data = buffer.getData();
    short *output = (short *)outputBuffer;    

    for (int i = 0; i < format.bufferSize; i++)
    {
        for (int j = 0; j < format.channels; j++)
        {
            short sample = data.at(i * format.channels + j);
            *(output + i * format.channels + j) = sample;
        }
    }

    return paContinue;
}

int record_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    AudioQueue<sample_type> *queue = (AudioQueue<sample_type> *)userData;
    AudioFormat format = queue->format;
    std::vector<sample_type> data(format.getSamplesPerBuffer());
    short *input = (short *)inputBuffer;    

    for (int i = 0; i < format.bufferSize; i++)
    {
        for (int j = 0; j < format.channels; j++)
        {
            short sample = *(input + i * format.channels + j);
            data.at(i * format.channels + j) = sample;
        }
    }

    AudioBuffer buffer(format, data);
    queue->_queue.push_back(buffer);
    return paContinue;
}
