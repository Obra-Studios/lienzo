#include "canvas.h"

namespace Lienzo {

Canvas::Canvas() {
}

void Canvas::addFrame(std::shared_ptr<Frame> frame) {
    frames.push_back(frame);
}

std::vector<std::shared_ptr<Frame>> Canvas::getFrames() const {
    return frames;
}

void Canvas::setViewport(double x, double y, double zoom) {
    viewportX = x;
    viewportY = y;
    this->zoom = zoom;
}

} // namespace Lienzo

