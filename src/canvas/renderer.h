#pragma once

#include "../core/frame.h"

namespace Lienzo {

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    void renderFrame(const Frame& frame);
    void clear();
    
private:
    // Rendering state
};

} // namespace Lienzo

