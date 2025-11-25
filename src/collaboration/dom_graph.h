#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Lienzo {

// Mergable DOM graph structure for collaboration
// Similar to Figma's approach

struct DOMNode {
    std::string id;
    std::string type;
    std::unordered_map<std::string, std::string> properties;
    std::vector<std::shared_ptr<DOMNode>> children;
    
    DOMNode(const std::string& id, const std::string& type)
        : id(id), type(type) {}
};

class DOMGraph {
public:
    DOMGraph();
    
    std::shared_ptr<DOMNode> createNode(const std::string& type);
    void addChild(std::shared_ptr<DOMNode> parent, std::shared_ptr<DOMNode> child);
    
    // Merge operations for collaboration
    void applyOperation(const std::string& operation);
    std::string serialize() const;
    void deserialize(const std::string& data);
    
private:
    std::shared_ptr<DOMNode> root;
    std::unordered_map<std::string, std::shared_ptr<DOMNode>> nodeMap;
};

} // namespace Lienzo

