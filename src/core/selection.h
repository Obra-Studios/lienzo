#pragma once

#include <vector>
#include <memory>
#include "vector.h"

namespace Lienzo {

class Selection {
public:
    void add(std::shared_ptr<VectorShape> shape);
    void remove(std::shared_ptr<VectorShape> shape);
    void clear();
    bool isEmpty() const;
    std::vector<std::shared_ptr<VectorShape>> getSelected() const;
    
private:
    std::vector<std::shared_ptr<VectorShape>> selected;
};

} // namespace Lienzo

