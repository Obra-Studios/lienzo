#include "frame.h"

namespace Lienzo {

Frame::Frame(double x, double y, double width, double height)
    : x(x), y(y), width(width), height(height) {
}

void Frame::setPosition(double x, double y) {
    this->x = x;
    this->y = y;
}

void Frame::setSize(double width, double height) {
    this->width = width;
    this->height = height;
}

void Frame::addShape(std::shared_ptr<VectorShape> shape) {
    shapes.push_back(shape);
}

std::vector<std::shared_ptr<VectorShape>> Frame::getShapes() const {
    return shapes;
}

bool Frame::contains(double px, double py) const {
    return px >= x && px <= x + width && py >= y && py <= y + height;
}

} // namespace Lienzo

