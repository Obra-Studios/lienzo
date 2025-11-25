#pragma once

#include <vector>
#include <memory>

namespace Lienzo {

struct Point {
    double x;
    double y;
    
    Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}
};

struct VectorPath {
    std::vector<Point> points;
    bool closed = false;
    
    void addPoint(const Point& point);
    void close();
};

class VectorShape {
public:
    virtual ~VectorShape() = default;
    virtual VectorPath getPath() const = 0;
    virtual void transform(double dx, double dy, double scale, double rotation) = 0;
    
protected:
    Point position;
    double rotation = 0.0;
    double scaleX = 1.0;
    double scaleY = 1.0;
};

} // namespace Lienzo

