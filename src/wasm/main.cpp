#include <emscripten.h>
#include "../core/frame.h"
#include "../canvas/canvas.h"
#include "crdt_bindings.h"

using namespace Lienzo;

// Forward declaration
extern "C" {
    void* crdt_manager_create(const char* siteId);
}

extern "C" {

// Legacy functions (kept for compatibility)
EMSCRIPTEN_KEEPALIVE
void* create_canvas() {
    return new Canvas();
}

EMSCRIPTEN_KEEPALIVE
void* create_frame(double x, double y, double width, double height) {
    return new Frame(x, y, width, height);
}

EMSCRIPTEN_KEEPALIVE
void add_frame_to_canvas(void* canvas_ptr, void* frame_ptr) {
    Canvas* canvas = static_cast<Canvas*>(canvas_ptr);
    Frame* frame = static_cast<Frame*>(frame_ptr);
    canvas->addFrame(std::shared_ptr<Frame>(frame));
}

} // extern "C"

int main() {
    // WASM module initialization
    // Initialize default CRDT manager
    crdt_manager_create("default");
    return 0;
}

