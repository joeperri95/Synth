cmake_minimum_required(VERSION "3.16")

find_package(SDL2 REQUIRED)
find_package(imgui REQUIRED)
find_package(implot REQUIRED)

find_package(spdlog REQUIRED)

if(FALSE)
set(Sources
    ../bindings/imgui_impl_sdl.h
    ../bindings/imgui_impl_sdl.cpp
    ../bindings/imgui_impl_sdlrenderer.h
    ../bindings/imgui_impl_sdlrenderer.cpp
    ../external/imnodes/imnodes.h
    ../external/imnodes/imnodes.cpp
    ../external/imnodes/imnodes_internal.h
)
endif()

set(Headers
    ../../common/AudioBuffer.h
    ../../common/AudioFormat.h 
    ../../common/AudioQueue.h
    ../../common/LockingRingBuffer.h
    ../Node.h
)

set(Sources
    ../Node.cpp
    ../../common/AudioParameter.cpp
)

set(NodeLinkLibraries
    -lm -lportaudio -lasound -lpthread spdlog::spdlog
)

include_directories(.)
include_directories(..)
include_directories(../..)
include_directories("../bindings")

add_compile_options(-pthread)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(fileSink_node SHARED FileSinkNode.cpp ../Node.cpp ${Headers})
target_link_libraries(fileSink_node ${NodeLinkLibraries})

add_library(fileSource_node SHARED FileSourceNode.cpp ../Node.cpp ${Headers})
target_link_libraries(fileSource_node ${NodeLinkLibraries})

add_library(mixer_node SHARED MixerNode.cpp ../Node.cpp ../../common/AudioParameter.cpp  ${Headers})
target_link_libraries(mixer_node ${NodeLinkLibraries})

add_library(passthrough_node SHARED PassthroughNode.cpp ../Node.cpp ${Headers})
target_link_libraries(passthrough_node ${NodeLinkLibraries})

add_library(sineSource_node SHARED SineSourceNode.cpp ../Node.cpp ../../common/AudioParameter.cpp ${Headers})
target_link_libraries(sineSource_node ${NodeLinkLibraries})

add_library(squareSource_node SHARED SquareSourceNode.cpp ../Node.cpp ../../common/AudioParameter.cpp ${Headers})
target_link_libraries(squareSource_node ${NodeLinkLibraries})

add_library(tremolo_node SHARED TremoloNode.cpp ../Node.cpp ../../common/AudioParameter.cpp ${Headers})
target_link_libraries(tremolo_node ${NodeLinkLibraries})

add_library(volume_node SHARED VolumeNode.cpp ../Node.cpp ../../common/AudioParameter.cpp ${Headers})
target_link_libraries(volume_node ${NodeLinkLibraries})