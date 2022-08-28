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

#include <portaudio.h>
#include <SDL.h>
#include <SDL_opengles.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "wav.h"
#include "AudioBuffer.h"
#include "AudioFormat.h"
#include "AudioQueue.h"
#include "effects.h"


using audio::AudioBuffer;
using audio::AudioFormat;
using audio::AudioQueue;

struct EffectController {
    EffectController() : m(), commands() {}
    std::mutex m;
    std::deque<std::string> commands;
};

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

int passthrough_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData);

void effect_thread(AudioQueue<int16_t> *in, AudioQueue<int16_t> *out, std::shared_ptr<EffectController>); 
void splitter(AudioQueue<int16_t> *input, AudioQueue<int16_t> *output1, AudioQueue<int16_t> *output2);

int main(int argc, char *argv[])
{
    std::cout << "Device initializing " << std::endl;

    AudioFormat format(CHANNELS, SAMPLE_RATE, BIT_DEPTH, BUFF_SIZE);
    AudioQueue<int16_t> audio_in, audio_out, effects, display;
    audio_in.format = format;
    audio_out.format = format;
    effects.format = format;
    display.format = format;
    std::shared_ptr<EffectController> controller= std::make_shared<EffectController>();

    PaStream *stream;
    PaStream *record_stream;

    std::cout << format.channels << " " << format.sampleRate << " " << format.bufferSize << std::endl;
    //build a track with channels
    //build a callback
    
    std::thread t(effect_thread, &audio_in, &effects, controller);
    std::thread t2(splitter, &effects, &audio_out, &display);

    Pa_Initialize();
    PaError err = Pa_OpenDefaultStream(&stream, 0, format.channels, paInt16, format.sampleRate, format.bufferSize, play_callback, (void *) &audio_out);
    PaError err2 = Pa_OpenDefaultStream(&record_stream, format.channels, 0, paInt16, format.sampleRate, format.bufferSize, record_callback, (void *) &audio_in);
    //PaError err3 = Pa_OpenDefaultStream(&record_stream, format.channels, format.channels, paInt16, format.sampleRate, format.bufferSize, passthrough_callback, &format);
    std::cout << "Write stream" << Pa_GetErrorText(err) << std::endl;
    std::cout << "Read stream" << Pa_GetErrorText(err2) << std::endl;

    Pa_StartStream(record_stream);
    Pa_StartStream(stream);

    std::string command;

    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Main window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Point points[1024];
    for(int i = 0; i < 1024; i++) {
        points[i] = {i + 20, 720 };
    }

    bool quit=false;
    while(!quit) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
        if (!display._queue.empty()) {
            AudioBuffer<int16_t> buffer = display._queue.front();
            display._queue.pop_front();
            std::vector<int16_t> data = buffer.getData();

            for(int i = 0; i < 1024; i++){
                int normalized_point = (800 * (((double) data.at(i)) - INT16_MIN) / (INT16_MAX - INT16_MIN));
                points[i] = {i + 20, normalized_point};
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawLines(renderer, points, 1024);

        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    while(1)
    {
        std::cin >> command;

        if(command.compare("quit") == 0) {
            break;
        } 
        else {
            {std::lock_guard(controller->m);
             controller->commands.push_back(command);
            }
        } 
    }

    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}

void splitter(AudioQueue<int16_t> *input, AudioQueue<int16_t> *output1, AudioQueue<int16_t> *output2)
{    
    while(1) {
    if(!input->_queue.empty()) {
        AudioBuffer<int16_t> sample = input->_queue.front();
        input->_queue.pop_front();
        output1->_queue.push_back(sample);
        output2->_queue.push_back(sample);
    }
    }
}

void effect_thread(AudioQueue<int16_t> *in, AudioQueue<int16_t> *out, std::shared_ptr<EffectController> controller) {

    std::string lastCommand;
    std::string currentCommand;
    double volume = 1.0;
    bool tremolo = false;
    bool vibrato = false;
    bool bitcrush = false;
    bool flanger = false;
    bool average = false;
    bool reverb = false;

    while (1) {
        std::lock_guard(controller->m);
        if (!controller->commands.empty()) {
            currentCommand = controller->commands.front();
            lastCommand = currentCommand;
            controller->commands.pop_front();

            if(lastCommand.compare("up") == 0) {
                volume += 0.1;
                std::cout << "Volume changes to " << volume << std::endl;
            } else if (lastCommand.compare("down") == 0) {
                volume -= 0.1;
                std::cout << "Volume changes to " << volume << std::endl;
            } else if (lastCommand.compare("tremolo") == 0) {
                tremolo = !tremolo;
                std::cout << "Tremolo has been set to " << tremolo << std::endl;
            } else if (lastCommand.compare("vibrato") == 0) {
                vibrato = !vibrato;
                std::cout << "Vibrato has been set to " << vibrato << std::endl;
            } else if (lastCommand.compare("bitcrush") == 0) {
                bitcrush = !bitcrush;
                std::cout << "bitcrush has been set to " << bitcrush << std::endl;
            } else if (lastCommand.compare("average") == 0) {
                average = !average;
                std::cout << "average has been set to " << average << std::endl;
            } else if (lastCommand.compare("reverb") == 0) {
                reverb = !reverb;
                std::cout << "reverb has been set to " << reverb << std::endl;
            } else if (lastCommand.compare("flanger") == 0) {
                flanger = !flanger;
                std::cout << "flanger has been set to " << flanger << std::endl;
            }
        }
        if (!in->_queue.empty()) {
            AudioBuffer last = in->_queue.front();
            AudioBuffer<int16_t> next(last.getFormat());

            in->_queue.pop_front();

            if(tremolo) {
               next = audio::effects::tremolo(last, 1);
               last = next;
            }
            if (vibrato) {
                next= audio::effects::vibrato(last, 20, 100);
                last = next;
            }
            if (reverb) {
                next = audio::effects::reverb(last, 200, 0.5);
                last = next;
            }
            if (flanger) {
                next = audio::effects::flanger(last, 200, 0.5, 0.4);
                last = next;
            }
            if (bitcrush) {
                next = audio::effects::bitcrush(last);
                last = next;
            }
            if (average) {
                next = audio::effects::average_filter(last, 21);
                last = next;
            }

            next = audio::effects::volume(last, volume);
            out->_queue.push_back(next);
            
            //out->_queue.push_back(buffer);
        }
    }
}

int play_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {

    AudioQueue<int16_t> *queue = (AudioQueue<int16_t> *)userData;

    if (queue->_queue.empty()) {
        return paContinue;
    }

    AudioFormat format = queue->format;
    AudioBuffer buffer = queue->_queue.front();
    queue->_queue.pop_front();
    std::vector<int16_t> data = buffer.getData();
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
    AudioQueue<int16_t> *queue = (AudioQueue<int16_t> *)userData;
    AudioFormat format = queue->format;
    std::vector<int16_t> data(format.getSamplesPerBuffer());
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

/*
int callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    AudioBuffer<int16_t> *player = (AudioBuffer<int16_t> *)userData;
    AudioFormat format = player->getFormat();
    std::vector<int16_t> data = player->getData();
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
    std::shared_ptr<AudioBuffer<int16_t>> player =*(std::shared_ptr<AudioBuffer<int16_t>>*) userData;
    AudioFormat format = player->getFormat();
    std::vector<int16_t> data(format.getSamplesPerBuffer());
    short *input = (short *)inputBuffer;    

    for (int i = 0; i < format.bufferSize; i++)
    {
        for (int j = 0; j < format.channels; j++)
        {
            data.push_back(*(input + i * format.channels + j));
        }
    }

    player->setData(data);
    return paContinue;
}

int passthrough_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long bufferSize,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    AudioFormat format = *(AudioFormat *) userData;
    short *input = (short *)inputBuffer;    
    short *output = (short *)outputBuffer;    

    for (int i = 0; i < format.bufferSize; i++)
    {
        for (int j = 0; j < format.channels; j++)
        {
            output[i*format.channels +j] = input[i*format.channels + j];
        }
    }

    return paContinue;
}
*/