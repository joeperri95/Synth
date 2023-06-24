cmake_minimum_required(VERSION "3.16")

find_package(SDL2 REQUIRED)
find_package(imgui REQUIRED)
find_package(implot REQUIRED)
find_package(spdlog REQUIRED)

set(Sources
    ../bindings/imgui_impl_sdl.h
    ../bindings/imgui_impl_sdl.cpp
    ../bindings/imgui_impl_sdlrenderer.h
    ../bindings/imgui_impl_sdlrenderer.cpp
    ../external/imnodes/imnodes.h
    ../external/imnodes/imnodes.cpp
    ../external/imnodes/imnodes_internal.h
    BasicPluginControl.cpp
)

set(Headers
    ../include/AudioBuffer.h
    ../include/AudioFormat.h 
    ../include/AudioQueue.h
    ../include/Node.h
)

include_directories(.)
include_directories(../include)
include_directories("../bindings")

add_compile_options(-pthread)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(${CMAKE_PROJECT_NAME}_node SHARED BasicPluginNode.cpp ${Headers})
add_library(${CMAKE_PROJECT_NAME}_control SHARED ${Sources} ../include/ControlWidget.h)

target_link_libraries(${CMAKE_PROJECT_NAME}_node -lm -lportaudio -lasound -lpthread 
                        spdlog::spdlog
                        )
target_link_libraries(${CMAKE_PROJECT_NAME}_control -lpthread 
                        SDL2::SDL2 
                        imgui::imgui
                        spdlog::spdlog
                        )
