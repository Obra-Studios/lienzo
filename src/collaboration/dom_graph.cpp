#include "dom_graph.h"
#include <sstream>

namespace Lienzo {

DOMGraph::DOMGraph() {
    root = std::make_shared<DOMNode>("root", "root");
    nodeMap["root"] = root;
}

std::shared_ptr<DOMNode> DOMGraph::createNode(const std::string& type) {
    // TODO: Generate unique ID
    std::string id = "node_" + std::to_string(nodeMap.size());
    auto node = std::make_shared<DOMNode>(id, type);
    nodeMap[id] = node;
    return node;
}

void DOMGraph::addChild(std::shared_ptr<DOMNode> parent, std::shared_ptr<DOMNode> child) {
    if (parent && child) {
        parent->children.push_back(child);
    }
}

void DOMGraph::applyOperation(const std::string& operation) {
    // TODO: Implement operation transformation
}

std::string DOMGraph::serialize() const {
    // TODO: Implement serialization
    return "";
}

void DOMGraph::deserialize(const std::string& data) {
    // TODO: Implement deserialization
}

} // namespace Lienzo

