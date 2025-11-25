#include "crdt.h"
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace Lienzo {

// CRDTNode implementation
CRDTNode::CRDTNode(const CRDTId& id, const std::string& type)
    : id(id), type(type), deleted(false), deletedTimestamp() {
}

void CRDTNode::markDeleted(const CRDTId& timestamp) {
    if (!deleted || timestamp.logicalClock > deletedTimestamp.logicalClock ||
        (timestamp.logicalClock == deletedTimestamp.logicalClock && 
         timestamp.siteId > deletedTimestamp.siteId)) {
        deleted = true;
        deletedTimestamp = timestamp;
    }
}

void CRDTNode::setProperty(const std::string& key, const std::string& value, 
                           const CRDTId& timestamp) {
    auto it = properties.find(key);
    if (it != properties.end()) {
        CRDTProperty<std::string> prop(value, timestamp);
        it->second.merge(prop);
    } else {
        properties[key] = CRDTProperty<std::string>(value, timestamp);
    }
}

std::string CRDTNode::getProperty(const std::string& key) const {
    auto it = properties.find(key);
    if (it != properties.end() && !it->second.timestamp.siteId.empty()) {
        return it->second.value;
    }
    return "";
}

bool CRDTNode::hasProperty(const std::string& key) const {
    auto it = properties.find(key);
    return it != properties.end() && !it->second.timestamp.siteId.empty();
}

void CRDTNode::addChild(const CRDTId& childId, const CRDTId& timestamp) {
    // Check if child already exists
    for (auto& child : children) {
        if (child.childId == childId) {
            // Re-add if it was deleted
            if (child.deleted) {
                if (timestamp.logicalClock > child.deletedTimestamp.logicalClock ||
                    (timestamp.logicalClock == child.deletedTimestamp.logicalClock &&
                     timestamp.siteId > child.deletedTimestamp.siteId)) {
                    child.deleted = false;
                    child.addedTimestamp = timestamp;
                    child.deletedTimestamp = CRDTId();
                }
            }
            return;
        }
    }
    
    // Add new child
    children.push_back(ChildEntry(childId, timestamp));
}

void CRDTNode::removeChild(const CRDTId& childId, const CRDTId& timestamp) {
    for (auto& child : children) {
        if (child.childId == childId) {
            if (!child.deleted || 
                timestamp.logicalClock > child.deletedTimestamp.logicalClock ||
                (timestamp.logicalClock == child.deletedTimestamp.logicalClock &&
                 timestamp.siteId > child.deletedTimestamp.siteId)) {
                child.deleted = true;
                child.deletedTimestamp = timestamp;
            }
            return;
        }
    }
}

std::vector<CRDTId> CRDTNode::getChildren() const {
    std::vector<CRDTId> result;
    for (const auto& child : children) {
        if (!child.deleted) {
            result.push_back(child.childId);
        }
    }
    return result;
}

void CRDTNode::merge(const CRDTNode& other) {
    if (other.id != id || other.type != type) {
        return; // Can only merge nodes with same ID and type
    }
    
    // Merge deletion state
    if (other.deleted) {
        markDeleted(other.deletedTimestamp);
    }
    
    // Merge properties (LWW)
    for (const auto& prop : other.properties) {
        auto it = properties.find(prop.first);
        if (it != properties.end()) {
            it->second.merge(prop.second);
        } else {
            properties[prop.first] = prop.second;
        }
    }
    
    // Merge children
    // For simplicity, we merge all child entries and let getChildren() filter deleted ones
    for (const auto& otherChild : other.children) {
        bool found = false;
        for (auto& child : children) {
            if (child.childId == otherChild.childId) {
                // Merge child state
                if (otherChild.deleted) {
                    removeChild(otherChild.childId, otherChild.deletedTimestamp);
                } else {
                    addChild(otherChild.childId, otherChild.addedTimestamp);
                }
                found = true;
                break;
            }
        }
        if (!found) {
            children.push_back(otherChild);
        }
    }
}

std::string CRDTNode::serialize() const {
    // TODO: Implement JSON serialization
    return "";
}

void CRDTNode::deserialize(const std::string& data) {
    // TODO: Implement JSON deserialization
}

// CRDTDocument implementation
CRDTDocument::CRDTDocument(const std::string& siteId)
    : siteId(siteId), logicalClock(0) {
    // Create root node
    rootId = generateId();
    auto root = std::make_shared<CRDTNode>(rootId, "root");
    nodes[rootId.toString()] = root;
}

CRDTId CRDTDocument::generateId() {
    return CRDTId(siteId, ++logicalClock);
}

CRDTId CRDTDocument::createNode(const std::string& type) {
    CRDTId id = generateId();
    auto node = std::make_shared<CRDTNode>(id, type);
    nodes[id.toString()] = node;
    return id;
}

CRDTId CRDTDocument::createNodeWithId(const CRDTId& id, const std::string& type) {
    auto node = std::make_shared<CRDTNode>(id, type);
    nodes[id.toString()] = node;
    // Update logical clock if needed
    if (id.siteId == siteId && id.logicalClock > logicalClock) {
        logicalClock = id.logicalClock;
    }
    return id;
}

std::shared_ptr<CRDTNode> CRDTDocument::getNode(const CRDTId& id) const {
    auto it = nodes.find(id.toString());
    if (it != nodes.end()) {
        return it->second;
    }
    return nullptr;
}

void CRDTDocument::deleteNode(const CRDTId& id) {
    auto node = getNode(id);
    if (node) {
        CRDTId timestamp = generateId();
        node->markDeleted(timestamp);
    }
}

void CRDTDocument::setNodeProperty(const CRDTId& nodeId, const std::string& key,
                                   const std::string& value) {
    auto node = getNode(nodeId);
    if (node) {
        CRDTId timestamp = generateId();
        node->setProperty(key, value, timestamp);
    }
}

std::string CRDTDocument::getNodeProperty(const CRDTId& nodeId, const std::string& key) const {
    auto node = getNode(nodeId);
    if (node) {
        return node->getProperty(key);
    }
    return "";
}

void CRDTDocument::addChild(const CRDTId& parentId, const CRDTId& childId) {
    auto parent = getNode(parentId);
    if (parent) {
        CRDTId timestamp = generateId();
        parent->addChild(childId, timestamp);
    }
}

void CRDTDocument::removeChild(const CRDTId& parentId, const CRDTId& childId) {
    auto parent = getNode(parentId);
    if (parent) {
        CRDTId timestamp = generateId();
        parent->removeChild(childId, timestamp);
    }
}

std::vector<CRDTId> CRDTDocument::getChildren(const CRDTId& parentId) const {
    auto parent = getNode(parentId);
    if (parent) {
        return parent->getChildren();
    }
    return std::vector<CRDTId>();
}

void CRDTDocument::merge(const CRDTDocument& other) {
    // Merge all nodes from other document
    for (const auto& pair : other.nodes) {
        auto it = nodes.find(pair.first);
        if (it != nodes.end()) {
            // Node exists, merge it
            it->second->merge(*pair.second);
        } else {
            // New node, add it
            nodes[pair.first] = std::make_shared<CRDTNode>(*pair.second);
        }
    }
    
    // Update logical clock if we received operations from our own site
    // (this handles the case where we receive our own operations back)
    for (const auto& pair : other.nodes) {
        CRDTId id;
        // Parse ID from string (format: "siteId:clock")
        size_t colonPos = pair.first.find(':');
        if (colonPos != std::string::npos) {
            std::string site = pair.first.substr(0, colonPos);
            if (site == siteId) {
                uint64_t clock = std::stoull(pair.first.substr(colonPos + 1));
                if (clock > logicalClock) {
                    logicalClock = clock;
                }
            }
        }
    }
}

std::string CRDTDocument::serialize() const {
    // TODO: Implement JSON serialization
    return "";
}

void CRDTDocument::deserialize(const std::string& data) {
    // TODO: Implement JSON deserialization
}

std::vector<CRDTId> CRDTDocument::getAllNodeIds() const {
    std::vector<CRDTId> result;
    for (const auto& pair : nodes) {
        CRDTId id;
        size_t colonPos = pair.first.find(':');
        if (colonPos != std::string::npos) {
            std::string site = pair.first.substr(0, colonPos);
            uint64_t clock = std::stoull(pair.first.substr(colonPos + 1));
            result.push_back(CRDTId(site, clock));
        }
    }
    return result;
}

} // namespace Lienzo

