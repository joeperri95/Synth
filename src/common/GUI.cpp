#include "GUI.h"
#include <SDL_video.h>
#include <iostream>
namespace ui {

GUI::GUI() {
    title = "synth";
    height = 800;
    width = 600;
    shouldQuit = false;
    this->controller = std::make_shared<PipelineController>();
    this->nodes = std::make_unique<nodes::NodeEditorWidget>(this->controller);
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

    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImNodes::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer_Init(renderer.get());
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

/*
    {
        static float f = 0.0f;
        bool b = false;

        ImGui::Begin("Control Panel"); 

        ImGui::SliderFloat("volume", &f, 0.0f, 1.0f);
        controller->volume = f;

        b = controller->flanger;
        ImGui::Checkbox("flanger", &b);
        controller->flanger = b;
        
        b = controller->reverb;
        ImGui::Checkbox("reverb", &b);
        controller->reverb = b;
        
        b = controller->average;
        ImGui::Checkbox("average", &b);
        controller->average = b;

        b = controller->bitcrush;
        ImGui::Checkbox("bitcrush", &b);
        controller->bitcrush = b;

        b = controller->tremolo;
        ImGui::Checkbox("tremolo", &b);
        controller->tremolo = b;

        b = controller->vibrato;
        ImGui::Checkbox("vibrato", &b);
        controller->vibrato = b;

        ImGui::End();
    }

    if (controller->flanger) {
        float f = 0.0f;
        int depth = 200;

        ImGui::Begin("Flanger"); 

        f = controller->flanger_freq;
        ImGui::SliderFloat("frequency", &f, 0.0f, 5.0f);
        controller->flanger_freq = f;

        f = controller->flanger_decay;
        ImGui::SliderFloat("decay", &f, 0.0f, 1.0f);
        controller->flanger_decay = f;

        depth = controller->flanger_depth;
        ImGui::SliderInt("depth", &depth, 100, 1000);
        controller->flanger_depth = depth;
        
        ImGui::End();
    }

    if (controller->vibrato) {
        float f = 0.0f;
        int depth = 20;

        ImGui::Begin("Vibrato"); 

        f = controller->vibrato_freq;
        ImGui::SliderFloat("frequency", &f, 0.0f, 20.0f);
        controller->vibrato_freq = f;

        depth = controller->vibrato_depth;
        ImGui::SliderInt("depth", &depth, 1, 200);
        controller->vibrato_depth = depth;
        
        ImGui::End();
    }
    
    if (controller->tremolo) {
        float f = 0.0f;
        int depth = 200;

        ImGui::Begin("Tremolo"); 

        f = controller->tremolo_freq;
        ImGui::SliderFloat("frequency", &f, 0.0f, 5.0f);
        controller->tremolo_freq = f;

        depth = controller->tremolo_depth;
        ImGui::SliderInt("depth", &depth, 100, 1000);
        controller->tremolo_depth = depth;
        
        ImGui::End();
    }
    
    if (controller->reverb) {
        int depth = 5;
        float f = 0.0f;

        ImGui::Begin("Reverb"); 

        f = controller->reverb_decay;
        ImGui::SliderFloat("decay", &f, 0.0f, 1.0f);
        controller->reverb_decay = f;

        depth = controller->reverb_depth;
        ImGui::SliderInt("depth", &depth, 1, 51);
        controller->reverb_depth = depth;
        
        ImGui::End();
    }
    
    if (controller->average) {
        int depth = 5;

        ImGui::Begin("Average"); 

        depth = controller->average_taps;
        ImGui::SliderInt("taps", &depth, 1, 51);
        controller->average_taps = depth;
        
        ImGui::End();
    }

    {
        int x_data[1024];
        int y_data[1024];
        std::vector<sample_type> data(1024);

        if(!queue->empty()) {
            sample_type sample = queue->pop();
            data.push_back(sample);
            if(data.size() >= 1024) {
                data.erase(data.begin());
            }
        }


        for(int i = 0; i < 1024; i++) {
            x_data[i] = i;
            y_data[i] = data.at(i);
        }

        ImGui::Begin("Plot");
        if (ImPlot::BeginPlot("Big Graph")) {
            ImPlot::SetupAxes("x", "y");
            ImPlot::SetupAxesLimits(0,1024, INT16_MIN, INT16_MAX);
            ImPlot::PlotLine("My Line Plot", x_data, y_data, 1024);
            ImPlot::EndPlot();
        }
        ImGui::End();
    }  
*/
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