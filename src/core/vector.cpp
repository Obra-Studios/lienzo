#include "vector.h"

namespace Lienzo {

void VectorPath::addPoint(const Point& point) {
    points.push_back(point);
}

void VectorPath::close() {
    closed = true;
}

} // namespace Lienzo

