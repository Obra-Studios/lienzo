#pragma once

#include <string>
#include <vector>
#include <memory>
#include "vector.h"

namespace Lienzo {

class Frame {
public:
    Frame(double x, double y, double width, double height);
    
    void setPosition(double x, double y);
    void setSize(double width, double height);
    
    double getX() const { return x; }
    double getY() const { return y; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    
    void addShape(std::shared_ptr<VectorShape> shape);
    std::vector<std::shared_ptr<VectorShape>> getShapes() const;
    
    bool contains(double px, double py) const;
    
private:
    double x, y;
    double width, height;
    std::vector<std::shared_ptr<VectorShape>> shapes;
};

} // namespace Lienzo

