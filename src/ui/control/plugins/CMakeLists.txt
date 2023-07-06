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
)

set(Headers
    ../ControlWidget.h
)

set(Sources
    ../../../common/AudioParameter.cpp
)

set(ControlLinkLibraries
    -lm 
    -lpthread 
    SDL2::SDL2 
    imgui::imgui
    implot::implot
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)

include_directories(.)
include_directories(..)
include_directories(../../..)
include_directories("../bindings")

add_compile_options(-pthread)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(sineSource_control SHARED SineControlWidget.cpp ${Sources} ${Headers})
target_link_libraries(sineSource_control ${ControlLinkLibraries})

if(FALSE)
add_library(fileSource_node SHARED FileSourceNode.cpp ../Node.cpp ${Headers})
target_link_libraries(fileSource_node ${ControlLinkLibraries})

add_library(tremolo_control SHARED TremoloWidget.cpp ${Sources} ${Headers})
target_link_libraries(tremolo_control ${ControlLinkLibraries})

add_library(volume_control SHARED VolumeWidget.cpp ${Sources} ${Headers})
target_link_libraries(volume_control ${ControlLinkLibraries})

add_library(vibrato_control SHARED VibratoWidget.cpp ${Sources} ${Headers})
target_link_libraries(vibrato_control ${ControlLinkLibraries})
endif()
