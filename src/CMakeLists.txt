cmake_minimum_required(VERSION "3.16")

find_package(SDL2 REQUIRED)
find_package(imgui REQUIRED)
find_package(implot REQUIRED)
find_package(spdlog REQUIRED)

set(Sources
    main.cpp
    common/GUI.cpp
    ../bindings/imgui_impl_sdl.h
    ../bindings/imgui_impl_sdl.cpp
    ../bindings/imgui_impl_sdlrenderer.h
    ../bindings/imgui_impl_sdlrenderer.cpp
    ../external/imnodes/imnodes.h
    ../external/imnodes/imnodes.cpp
    ../external/imnodes/imnodes_internal.h
    ui/nodes/NodeEditorWidget.cpp
    ui/nodes/NodeWidgetFactory.cpp
    ui/nodes/VolumeNodeWidget.cpp
    ui/nodes/SinkNodeWidget.cpp
    ui/nodes/SourceNodeWidget.cpp
    ui/nodes/MixerNodeWidget.cpp
    ui/control/ControlWidgetFactory.cpp
    ui/control/VolumeWidget.cpp
    ui/control/SineControlWidget.cpp
    pipeline/Pipeline.cpp
    pipeline/NodeFactory.cpp
    pipeline/VolumeNode.cpp
    pipeline/SineSourceNode.cpp
    pipeline/SquareSourceNode.cpp
    pipeline/PASinkNode.cpp
    pipeline/PASourceNode.cpp
    pipeline/MixerNode.cpp
    pipeline/Node.cpp
    common/AudioDevice.cpp
    AudioParameter.cpp
    common/PipelineController.cpp
    common/LockingRingBuffer.h
    common/AttributeInfo.cpp
)

set(Headers
    common/AudioBuffer.h
    AudioFormat.h 
    AudioQueue.h
    common/GUI.h
)

include_directories(.)
include_directories("../bindings")

add_compile_options(-pthread)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(synth ${Sources} ${Headers})

target_link_libraries(synth -lm -lportaudio -lasound -lpthread 
                        SDL2::SDL2 
                        imgui::imgui
                        implot::implot
                        spdlog::spdlog
                        )
