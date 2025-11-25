#pragma once

#include "../collaboration/crdt.h"
#include "vector.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace Lienzo {

// CRDT-aware vector shape
// Wraps VectorShape with CRDT properties for collaboration
class CRDTVectorShape {
public:
    CRDTVectorShape(const CRDTId& id, std::shared_ptr<VectorShape> shape);
    virtual ~CRDTVectorShape() = default;
    
    CRDTId getId() const { return id; }
    std::shared_ptr<VectorShape> getShape() const { return shape; }
    
    // Sync with CRDT node
    void syncFromCRDTNode(const CRDTNode& node);
    void syncToCRDTNode(CRDTNode& node) const;
    
    // Property getters/setters that update CRDT
    void setPosition(double x, double y, CRDTDocument& doc);
    Point getPosition() const;
    
    void setRotation(double rotation, CRDTDocument& doc);
    double getRotation() const;
    
    void setScale(double scaleX, double scaleY, CRDTDocument& doc);
    double getScaleX() const;
    double getScaleY() const;
    
    // Transform operations that generate CRDT operations
    void transform(double dx, double dy, double scale, double rotation, CRDTDocument& doc);
    
private:
    CRDTId id;
    std::shared_ptr<VectorShape> shape;
    
    // Helper to parse/format property values
    static std::string pointToString(const Point& p);
    static Point pointFromString(const std::string& str);
};

// CRDT-aware frame
class CRDTFrame {
public:
    CRDTFrame(const CRDTId& id, double x, double y, double width, double height);
    
    CRDTId getId() const { return id; }
    
    // Sync with CRDT node
    void syncFromCRDTNode(const CRDTNode& node);
    void syncToCRDTNode(CRDTNode& node, CRDTDocument& doc) const;
    
    // Property operations that update CRDT
    void setPosition(double x, double y, CRDTDocument& doc);
    void setSize(double width, double height, CRDTDocument& doc);
    
    double getX() const { return x; }
    double getY() const { return y; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    
    // Shape management
    void addShape(const CRDTId& shapeId, CRDTDocument& doc);
    void removeShape(const CRDTId& shapeId, CRDTDocument& doc);
    std::vector<CRDTId> getShapeIds() const;
    
    bool contains(double px, double py) const;
    
private:
    CRDTId id;
    double x, y;
    double width, height;
    std::vector<CRDTId> shapeIds;
};

// Manager that bridges vector shapes and CRDT document
class VectorCRDTManager {
public:
    VectorCRDTManager(const std::string& siteId);
    
    // Document access
    CRDTDocument& getDocument() { return document; }
    const CRDTDocument& getDocument() const { return document; }
    
    // Frame operations
    CRDTId createFrame(double x, double y, double width, double height);
    std::shared_ptr<CRDTFrame> getFrame(const CRDTId& frameId);
    void deleteFrame(const CRDTId& frameId);
    
    // Shape operations
    CRDTId createShape(const CRDTId& frameId, std::shared_ptr<VectorShape> shape);
    std::shared_ptr<CRDTVectorShape> getShape(const CRDTId& shapeId);
    void deleteShape(const CRDTId& shapeId);
    
    // Transform operations
    void transformShape(const CRDTId& shapeId, double dx, double dy, 
                       double scale, double rotation);
    
    // Merge with another document state
    void merge(const VectorCRDTManager& other);
    
    // Get all frames
    std::vector<CRDTId> getAllFrames() const;
    
    // Get all shapes in a frame
    std::vector<CRDTId> getShapesInFrame(const CRDTId& frameId) const;
    
private:
    CRDTDocument document;
    std::unordered_map<std::string, std::shared_ptr<CRDTFrame>> frames;
    std::unordered_map<std::string, std::shared_ptr<CRDTVectorShape>> shapes;
    
    void rebuildFromDocument();
    CRDTId createCRDTNodeForFrame(double x, double y, double width, double height);
    CRDTId createCRDTNodeForShape(const CRDTId& frameId, std::shared_ptr<VectorShape> shape);
};

} // namespace Lienzo

