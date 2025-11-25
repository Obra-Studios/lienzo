#include "selection.h"
#include <algorithm>

namespace Lienzo {

void Selection::add(std::shared_ptr<VectorShape> shape) {
    selected.push_back(shape);
}

void Selection::remove(std::shared_ptr<VectorShape> shape) {
    selected.erase(
        std::remove(selected.begin(), selected.end(), shape),
        selected.end()
    );
}

void Selection::clear() {
    selected.clear();
}

bool Selection::isEmpty() const {
    return selected.empty();
}

std::vector<std::shared_ptr<VectorShape>> Selection::getSelected() const {
    return selected;
}

} // namespace Lienzo

