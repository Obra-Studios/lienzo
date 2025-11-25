#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>

namespace Lienzo {

// Unique identifier for CRDT nodes
// Format: siteId:logicalClock (e.g., "user1:42")
struct CRDTId {
    std::string siteId;      // Unique identifier for the client/site
    uint64_t logicalClock;   // Logical clock for ordering operations
    
    CRDTId() : siteId(""), logicalClock(0) {}
    CRDTId(const std::string& site, uint64_t clock) : siteId(site), logicalClock(clock) {}
    
    std::string toString() const {
        return siteId + ":" + std::to_string(logicalClock);
    }
    
    bool operator==(const CRDTId& other) const {
        return siteId == other.siteId && logicalClock == other.logicalClock;
    }
    
    bool operator<(const CRDTId& other) const {
        if (siteId != other.siteId) {
            return siteId < other.siteId;
        }
        return logicalClock < other.logicalClock;
    }
};

// Property value with timestamp for Last-Write-Wins (LWW)
template<typename T>
struct CRDTProperty {
    T value;
    CRDTId timestamp;  // ID of the operation that set this value
    
    CRDTProperty() : value(T()), timestamp() {}
    CRDTProperty(const T& val, const CRDTId& ts) : value(val), timestamp(ts) {}
    
    // Merge: keep the value with the later timestamp
    void merge(const CRDTProperty<T>& other) {
        if (other.timestamp.logicalClock > timestamp.logicalClock ||
            (other.timestamp.logicalClock == timestamp.logicalClock && 
             other.timestamp.siteId > timestamp.siteId)) {
            value = other.value;
            timestamp = other.timestamp;
        }
    }
};

// Base class for all CRDT nodes
class CRDTNode {
public:
    CRDTNode(const CRDTId& id, const std::string& type);
    virtual ~CRDTNode() = default;
    
    // CRDT properties
    CRDTId getId() const { return id; }
    std::string getType() const { return type; }
    bool isDeleted() const { return deleted; }
    void markDeleted(const CRDTId& timestamp);
    
    // Property management (Last-Write-Wins)
    void setProperty(const std::string& key, const std::string& value, const CRDTId& timestamp);
    std::string getProperty(const std::string& key) const;
    bool hasProperty(const std::string& key) const;
    
    // Children management (ordered list CRDT)
    void addChild(const CRDTId& childId, const CRDTId& timestamp);
    void removeChild(const CRDTId& childId, const CRDTId& timestamp);
    std::vector<CRDTId> getChildren() const;
    
    // Merge this node with another node's state
    void merge(const CRDTNode& other);
    
    // Serialization
    virtual std::string serialize() const;
    virtual void deserialize(const std::string& data);
    
protected:
    CRDTId id;
    std::string type;
    bool deleted;
    CRDTId deletedTimestamp;
    
    // Properties with LWW semantics
    std::unordered_map<std::string, CRDTProperty<std::string>> properties;
    
    // Children with tombstones
    struct ChildEntry {
        CRDTId childId;
        CRDTId addedTimestamp;
        bool deleted;
        CRDTId deletedTimestamp;
        
        ChildEntry(const CRDTId& id, const CRDTId& ts) 
            : childId(id), addedTimestamp(ts), deleted(false), deletedTimestamp() {}
    };
    std::vector<ChildEntry> children;
};

// CRDT-based document graph
class CRDTDocument {
public:
    CRDTDocument(const std::string& siteId);
    
    // Node operations
    CRDTId createNode(const std::string& type);
    CRDTId createNodeWithId(const CRDTId& id, const std::string& type);
    std::shared_ptr<CRDTNode> getNode(const CRDTId& id) const;
    void deleteNode(const CRDTId& id);
    
    // Property operations
    void setNodeProperty(const CRDTId& nodeId, const std::string& key, 
                       const std::string& value);
    std::string getNodeProperty(const CRDTId& nodeId, const std::string& key) const;
    
    // Hierarchy operations
    void addChild(const CRDTId& parentId, const CRDTId& childId);
    void removeChild(const CRDTId& parentId, const CRDTId& childId);
    std::vector<CRDTId> getChildren(const CRDTId& parentId) const;
    
    // Merge with another document state
    void merge(const CRDTDocument& other);
    
    // Get root node
    CRDTId getRootId() const { return rootId; }
    
    // Serialization for network sync
    std::string serialize() const;
    void deserialize(const std::string& data);
    
    // Get all nodes (for iteration)
    std::vector<CRDTId> getAllNodeIds() const;
    
private:
    std::string siteId;
    uint64_t logicalClock;
    CRDTId rootId;
    std::unordered_map<std::string, std::shared_ptr<CRDTNode>> nodes; // key: id.toString()
    
    CRDTId generateId();
    void ensureNodeExists(const CRDTId& id, const std::string& type);
};

} // namespace Lienzo

