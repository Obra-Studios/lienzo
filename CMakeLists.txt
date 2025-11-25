cmake_minimum_required(VERSION 3.15)
project(Lienzo VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten configuration
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1")
endif()

# Source files
set(CORE_SOURCES
    src/core/vector.cpp
    src/core/selection.cpp
    src/core/frame.cpp
    src/core/vector_crdt.cpp
)

set(CANVAS_SOURCES
    src/canvas/renderer.cpp
    src/canvas/canvas.cpp
)

set(COLLABORATION_SOURCES
    src/collaboration/dom_graph.cpp
    src/collaboration/crdt.cpp
)

set(PLUGIN_SOURCES
    src/plugins/plugin.cpp
)

set(AI_SOURCES
    src/ai/chat.cpp
)

# Include directories
include_directories(
    src/core
    src/canvas
    src/collaboration
    src/plugins
    src/ai
)

# Create library
add_library(lienzo_core STATIC
    ${CORE_SOURCES}
    ${CANVAS_SOURCES}
    ${COLLABORATION_SOURCES}
    ${PLUGIN_SOURCES}
    ${AI_SOURCES}
)

# Main executable (WASM module)
if(EMSCRIPTEN)
    add_executable(lienzo
        src/wasm/main.cpp
        src/wasm/crdt_bindings.cpp
    )
    target_link_libraries(lienzo lienzo_core)
else()
    # Native build for testing
    add_executable(lienzo_test
        src/wasm/main.cpp
    )
    target_link_libraries(lienzo_test lienzo_core)
endif()

