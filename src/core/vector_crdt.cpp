#include "vector_crdt.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace Lienzo {

// CRDTVectorShape implementation
CRDTVectorShape::CRDTVectorShape(const CRDTId& id, std::shared_ptr<VectorShape> shape)
    : id(id), shape(shape) {
}

void CRDTVectorShape::syncFromCRDTNode(const CRDTNode& node) {
    // Read position
    if (node.hasProperty("x") && node.hasProperty("y")) {
        double x = std::stod(node.getProperty("x"));
        double y = std::stod(node.getProperty("y"));
        shape->transform(x, y, 1.0, 0.0); // TODO: Better transform API
    }
    
    // Read rotation
    if (node.hasProperty("rotation")) {
        double rotation = std::stod(node.getProperty("rotation"));
        // TODO: Apply rotation
    }
    
    // Read scale
    if (node.hasProperty("scaleX") && node.hasProperty("scaleY")) {
        double scaleX = std::stod(node.getProperty("scaleX"));
        double scaleY = std::stod(node.getProperty("scaleY"));
        // TODO: Apply scale
    }
}

void CRDTVectorShape::syncToCRDTNode(CRDTNode& node, CRDTDocument& doc) const {
    Point pos = getPosition();
    doc.setNodeProperty(id, "x", std::to_string(pos.x));
    doc.setNodeProperty(id, "y", std::to_string(pos.y));
    doc.setNodeProperty(id, "rotation", std::to_string(getRotation()));
    doc.setNodeProperty(id, "scaleX", std::to_string(getScaleX()));
    doc.setNodeProperty(id, "scaleY", std::to_string(getScaleY()));
}

void CRDTVectorShape::setPosition(double x, double y, CRDTDocument& doc) {
    doc.setNodeProperty(id, "x", std::to_string(x));
    doc.setNodeProperty(id, "y", std::to_string(y));
    // Update shape immediately for local responsiveness
    shape->transform(x, y, 1.0, 0.0); // TODO: Better API
}

Point CRDTVectorShape::getPosition() const {
    // Read from shape or from CRDT? For now, shape is source of truth
    // In full implementation, would read from CRDT node
    return Point(0, 0); // TODO: Get from shape
}

void CRDTVectorShape::setRotation(double rotation, CRDTDocument& doc) {
    doc.setNodeProperty(id, "rotation", std::to_string(rotation));
}

double CRDTVectorShape::getRotation() const {
    return 0.0; // TODO: Get from shape
}

void CRDTVectorShape::setScale(double scaleX, double scaleY, CRDTDocument& doc) {
    doc.setNodeProperty(id, "scaleX", std::to_string(scaleX));
    doc.setNodeProperty(id, "scaleY", std::to_string(scaleY));
}

double CRDTVectorShape::getScaleX() const {
    return 1.0; // TODO: Get from shape
}

double CRDTVectorShape::getScaleY() const {
    return 1.0; // TODO: Get from shape
}

void CRDTVectorShape::transform(double dx, double dy, double scale, 
                                double rotation, CRDTDocument& doc) {
    // Update CRDT properties
    Point currentPos = getPosition();
    doc.setNodeProperty(id, "x", std::to_string(currentPos.x + dx));
    doc.setNodeProperty(id, "y", std::to_string(currentPos.y + dy));
    doc.setNodeProperty(id, "rotation", std::to_string(getRotation() + rotation));
    double newScaleX = getScaleX() * scale;
    double newScaleY = getScaleY() * scale;
    doc.setNodeProperty(id, "scaleX", std::to_string(newScaleX));
    doc.setNodeProperty(id, "scaleY", std::to_string(newScaleY));
    
    // Update shape
    shape->transform(dx, dy, scale, rotation);
}

std::string CRDTVectorShape::pointToString(const Point& p) {
    return std::to_string(p.x) + "," + std::to_string(p.y);
}

Point CRDTVectorShape::pointFromString(const std::string& str) {
    size_t commaPos = str.find(',');
    if (commaPos != std::string::npos) {
        double x = std::stod(str.substr(0, commaPos));
        double y = std::stod(str.substr(commaPos + 1));
        return Point(x, y);
    }
    return Point(0, 0);
}

// CRDTFrame implementation
CRDTFrame::CRDTFrame(const CRDTId& id, double x, double y, double width, double height)
    : id(id), x(x), y(y), width(width), height(height) {
}

void CRDTFrame::syncFromCRDTNode(const CRDTNode& node) {
    if (node.hasProperty("x")) {
        x = std::stod(node.getProperty("x"));
    }
    if (node.hasProperty("y")) {
        y = std::stod(node.getProperty("y"));
    }
    if (node.hasProperty("width")) {
        width = std::stod(node.getProperty("width"));
    }
    if (node.hasProperty("height")) {
        height = std::stod(node.getProperty("height"));
    }
    
    // Sync children (shapes)
    shapeIds = node.getChildren();
}

void CRDTFrame::syncToCRDTNode(CRDTNode& node, CRDTDocument& doc) const {
    // Properties are set via CRDTDocument, not directly on node
    // This is called during sync, but properties should already be in document
    (void)node; // Unused parameter
    (void)doc;  // Unused parameter
}

void CRDTFrame::setPosition(double x, double y, CRDTDocument& doc) {
    this->x = x;
    this->y = y;
    doc.setNodeProperty(id, "x", std::to_string(x));
    doc.setNodeProperty(id, "y", std::to_string(y));
}

void CRDTFrame::setSize(double width, double height, CRDTDocument& doc) {
    this->width = width;
    this->height = height;
    doc.setNodeProperty(id, "width", std::to_string(width));
    doc.setNodeProperty(id, "height", std::to_string(height));
}

void CRDTFrame::addShape(const CRDTId& shapeId, CRDTDocument& doc) {
    // Check if already added
    for (const auto& id : shapeIds) {
        if (id == shapeId) {
            return;
        }
    }
    shapeIds.push_back(shapeId);
    doc.addChild(id, shapeId);
}

void CRDTFrame::removeShape(const CRDTId& shapeId, CRDTDocument& doc) {
    shapeIds.erase(
        std::remove(shapeIds.begin(), shapeIds.end(), shapeId),
        shapeIds.end()
    );
    doc.removeChild(id, shapeId);
}

std::vector<CRDTId> CRDTFrame::getShapeIds() const {
    return shapeIds;
}

bool CRDTFrame::contains(double px, double py) const {
    return px >= x && px <= x + width && py >= y && py <= y + height;
}

// VectorCRDTManager implementation
VectorCRDTManager::VectorCRDTManager(const std::string& siteId)
    : document(siteId) {
    rebuildFromDocument();
}

CRDTId VectorCRDTManager::createFrame(double x, double y, double width, double height) {
    CRDTId frameId = createCRDTNodeForFrame(x, y, width, height);
    auto frame = std::make_shared<CRDTFrame>(frameId, x, y, width, height);
    frames[frameId.toString()] = frame;
    
    // Add to root
    document.addChild(document.getRootId(), frameId);
    
    return frameId;
}

std::shared_ptr<CRDTFrame> VectorCRDTManager::getFrame(const CRDTId& frameId) {
    auto it = frames.find(frameId.toString());
    if (it != frames.end()) {
        return it->second;
    }
    return nullptr;
}

void VectorCRDTManager::deleteFrame(const CRDTId& frameId) {
    document.deleteNode(frameId);
    frames.erase(frameId.toString());
}

CRDTId VectorCRDTManager::createShape(const CRDTId& frameId, 
                                      std::shared_ptr<VectorShape> shape) {
    CRDTId shapeId = createCRDTNodeForShape(frameId, shape);
    auto crdtShape = std::make_shared<CRDTVectorShape>(shapeId, shape);
    shapes[shapeId.toString()] = crdtShape;
    
    // Add to frame
    auto frame = getFrame(frameId);
    if (frame) {
        frame->addShape(shapeId, document);
    }
    
    return shapeId;
}

std::shared_ptr<CRDTVectorShape> VectorCRDTManager::getShape(const CRDTId& shapeId) {
    auto it = shapes.find(shapeId.toString());
    if (it != shapes.end()) {
        return it->second;
    }
    return nullptr;
}

void VectorCRDTManager::deleteShape(const CRDTId& shapeId) {
    document.deleteNode(shapeId);
    shapes.erase(shapeId.toString());
}

void VectorCRDTManager::transformShape(const CRDTId& shapeId, double dx, double dy,
                                       double scale, double rotation) {
    auto shape = getShape(shapeId);
    if (shape) {
        shape->transform(dx, dy, scale, rotation, document);
    }
}

void VectorCRDTManager::merge(const VectorCRDTManager& other) {
    document.merge(other.document);
    rebuildFromDocument();
}

std::vector<CRDTId> VectorCRDTManager::getAllFrames() const {
    return document.getChildren(document.getRootId());
}

std::vector<CRDTId> VectorCRDTManager::getShapesInFrame(const CRDTId& frameId) const {
    return document.getChildren(frameId);
}

void VectorCRDTManager::rebuildFromDocument() {
    // Rebuild frames and shapes from CRDT document
    // This is called after merge to sync local state
    frames.clear();
    shapes.clear();
    
    // Rebuild frames
    auto frameIds = document.getChildren(document.getRootId());
    for (const auto& frameId : frameIds) {
        auto node = document.getNode(frameId);
        if (node && !node->isDeleted()) {
            double x = 0, y = 0, width = 100, height = 100;
            if (node->hasProperty("x")) x = std::stod(node->getProperty("x"));
            if (node->hasProperty("y")) y = std::stod(node->getProperty("y"));
            if (node->hasProperty("width")) width = std::stod(node->getProperty("width"));
            if (node->hasProperty("height")) height = std::stod(node->getProperty("height"));
            
            auto frame = std::make_shared<CRDTFrame>(frameId, x, y, width, height);
            frame->syncFromCRDTNode(*node);
            frames[frameId.toString()] = frame;
        }
    }
    
    // TODO: Rebuild shapes (would need shape type information)
}

CRDTId VectorCRDTManager::createCRDTNodeForFrame(double x, double y, 
                                                  double width, double height) {
    CRDTId frameId = document.createNode("frame");
    document.setNodeProperty(frameId, "x", std::to_string(x));
    document.setNodeProperty(frameId, "y", std::to_string(y));
    document.setNodeProperty(frameId, "width", std::to_string(width));
    document.setNodeProperty(frameId, "height", std::to_string(height));
    return frameId;
}

CRDTId VectorCRDTManager::createCRDTNodeForShape(const CRDTId& frameId,
                                                  std::shared_ptr<VectorShape> shape) {
    CRDTId shapeId = document.createNode("shape");
    // Shape properties will be set by CRDTVectorShape
    return shapeId;
}

} // namespace Lienzo

