#include "GUI.h"
#include <SDL_video.h>
#include <iostream>
namespace ui {

GUI::GUI() {
    title = "synth";
    height = 800;
    width = 600;
    shouldQuit = false;
    this->controller = nullptr;
    this->nodes = nullptr;
}

GUI::~GUI() {

}

void GUI::shut_down() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();

    SDL_DestroyRenderer(renderer.get());
    SDL_DestroyWindow(window.get());
    SDL_Quit();
}
void GUI::initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    window = std::unique_ptr<SDL_Window, sdl_deleter>(SDL_CreateWindow(this->title.c_str(),
                                                                            SDL_WINDOWPOS_CENTERED,
                                                                            SDL_WINDOWPOS_CENTERED,
                                                                            this->height, this->width,
                                                                            SDL_WINDOW_SHOWN |
                                                                            SDL_WINDOW_RESIZABLE
                                                                            ));

    renderer = std::unique_ptr<SDL_Renderer, sdl_deleter>(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
    IMGUI_CHECKVERSION();

    this->context = {
        .imgui_context = ImGui::CreateContext(),
        .imnodes_context = ImNodes::CreateContext(),
        .implot_context = ImPlot::CreateContext()
    };

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer_Init(renderer.get());

    this->controller = std::make_shared<PipelineController>(context);
    this->nodes = std::make_unique<nodes::NodeEditorWidget>(this->controller);
}

void GUI::handle_event() {
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            shouldQuit = true;
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window.get())) {
            shouldQuit = true;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_DELETE:
                    spdlog::info("Delete key pressed");
                break;
                default:
                break;
            }
        }
    }
}

void GUI::render() {

    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImVec4 clear_color = ImVec4(0.15f, 0.15f, 0.40f, 1.00f);

    nodes->render();

    for (auto it: this->controller->getWidgets()) {
        it->render();
    }

    // Rendering
    ImGui::Render();
    SDL_SetRenderDrawColor(renderer.get(), (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
    SDL_RenderClear(renderer.get());
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer.get());
}

}// namespace ui
