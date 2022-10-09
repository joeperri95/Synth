#pragma once

#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_opengles.h>
#include "implot.h"
#include "../external/imnodes/imnodes.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "sample_type.h"
#include "EffectController.h"
#include "AudioQueue.h"
#include "AudioBuffer.h"
#include "ui/ControlWidget.h"
#include "ui/VolumeWidget.h"
#include "ui/VolumeNodeWidget.h"
#include "ui/NodeEditorWidget.h"

struct sdl_deleter
{
    void operator()(SDL_Window *p) const { SDL_DestroyWindow(p); }
    void operator()(SDL_Renderer *p) const { SDL_DestroyRenderer(p); }
    void operator()(SDL_Texture *p) const { SDL_DestroyTexture(p); }
};

namespace ui {

class GUI{

public:
    GUI();
    ~GUI();

    void initialize(std::shared_ptr<EffectController> controller, audio::AudioQueue<sample_type> *queue, std::shared_ptr<ui::VolumeWidget> v);
    void shut_down();
    void handle_event();
    void render();

    bool shouldQuit;
private:
    std::unique_ptr<SDL_Renderer, sdl_deleter> renderer; 
    std::unique_ptr<SDL_Window, sdl_deleter> window; 
    SDL_Event event;
    std::string title;
    int height;
    int width;
    std::shared_ptr<EffectController> controller;
    audio::AudioQueue<sample_type> *queue;
    std::shared_ptr<ui::VolumeWidget> v;
    NodeEditorWidget nodes;
};
    
}