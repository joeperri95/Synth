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
#include "AudioQueue.h"
#include "PipelineController.h"
#include "DisplayContext.h"
#include "ui/control/ControlWidget.h"
#include "ui/nodes/NodeEditorWidget.h"

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

    void initialize();
    void shut_down();
    void handle_event();
    void render();

    bool shouldQuit;
private:
    // common
    std::unique_ptr<SDL_Renderer, sdl_deleter> renderer;
    std::unique_ptr<SDL_Window, sdl_deleter> window;
    SDL_Event event;
    std::string title;
    int height;
    int width;

    std::unique_ptr<nodes::NodeEditorWidget> nodes;
    DisplayContext context;

    std::vector<std::unique_ptr<ControlWidget>> widgets;
    std::shared_ptr<PipelineController> controller;
};

}
