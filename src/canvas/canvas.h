#pragma once

#include <vector>
#include <memory>
#include "../core/frame.h"

namespace Lienzo {

class Canvas {
public:
    Canvas();
    
    void addFrame(std::shared_ptr<Frame> frame);
    std::vector<std::shared_ptr<Frame>> getFrames() const;
    
    void setViewport(double x, double y, double zoom);
    double getZoom() const { return zoom; }
    
private:
    std::vector<std::shared_ptr<Frame>> frames;
    double viewportX = 0.0;
    double viewportY = 0.0;
    double zoom = 1.0;
};

} // namespace Lienzo

