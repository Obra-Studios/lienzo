#include "crdt_bindings.h"
#include <emscripten.h>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace Lienzo;

// Global manager instance
static VectorCRDTManager* g_manager = nullptr;

extern "C" {

// Initialize the CRDT manager
EMSCRIPTEN_KEEPALIVE
void* crdt_manager_create(const char* siteId) {
    if (g_manager) {
        delete g_manager;
    }
    g_manager = new VectorCRDTManager(std::string(siteId));
    return g_manager;
}

// Get the global manager
EMSCRIPTEN_KEEPALIVE
void* crdt_manager_get() {
    if (!g_manager) {
        // Create default manager if none exists
        g_manager = new VectorCRDTManager("default");
    }
    return g_manager;
}

// Frame operations
EMSCRIPTEN_KEEPALIVE
const char* crdt_create_frame(double x, double y, double width, double height) {
    if (!g_manager) return nullptr;
    
    CRDTId frameId = g_manager->createFrame(x, y, width, height);
    std::string idStr = crdtIdToString(frameId);
    
    // Allocate memory for the string (caller must free)
    char* result = (char*)malloc(idStr.length() + 1);
    strcpy(result, idStr.c_str());
    return result;
}

EMSCRIPTEN_KEEPALIVE
double crdt_frame_get_x(const char* frameIdStr) {
    if (!g_manager) return 0.0;
    CRDTId frameId = stringToCRDTId(frameIdStr);
    auto frame = g_manager->getFrame(frameId);
    if (frame) {
        return frame->getX();
    }
    return 0.0;
}

EMSCRIPTEN_KEEPALIVE
double crdt_frame_get_y(const char* frameIdStr) {
    if (!g_manager) return 0.0;
    CRDTId frameId = stringToCRDTId(frameIdStr);
    auto frame = g_manager->getFrame(frameId);
    if (frame) {
        return frame->getY();
    }
    return 0.0;
}

EMSCRIPTEN_KEEPALIVE
double crdt_frame_get_width(const char* frameIdStr) {
    if (!g_manager) return 0.0;
    CRDTId frameId = stringToCRDTId(frameIdStr);
    auto frame = g_manager->getFrame(frameId);
    if (frame) {
        return frame->getWidth();
    }
    return 0.0;
}

EMSCRIPTEN_KEEPALIVE
double crdt_frame_get_height(const char* frameIdStr) {
    if (!g_manager) return 0.0;
    CRDTId frameId = stringToCRDTId(frameIdStr);
    auto frame = g_manager->getFrame(frameId);
    if (frame) {
        return frame->getHeight();
    }
    return 0.0;
}

EMSCRIPTEN_KEEPALIVE
void crdt_frame_set_position(const char* frameIdStr, double x, double y) {
    if (!g_manager) return;
    CRDTId frameId = stringToCRDTId(frameIdStr);
    auto frame = g_manager->getFrame(frameId);
    if (frame) {
        frame->setPosition(x, y, g_manager->getDocument());
    }
}

EMSCRIPTEN_KEEPALIVE
void crdt_frame_set_size(const char* frameIdStr, double width, double height) {
    if (!g_manager) return;
    CRDTId frameId = stringToCRDTId(frameIdStr);
    auto frame = g_manager->getFrame(frameId);
    if (frame) {
        frame->setSize(width, height, g_manager->getDocument());
    }
}

EMSCRIPTEN_KEEPALIVE
void crdt_frame_delete(const char* frameIdStr) {
    if (!g_manager) return;
    CRDTId frameId = stringToCRDTId(frameIdStr);
    g_manager->deleteFrame(frameId);
}

// Get all frame IDs
EMSCRIPTEN_KEEPALIVE
void crdt_get_all_frames(char* buffer, int bufferSize) {
    if (!g_manager) {
        buffer[0] = '\0';
        return;
    }
    
    auto frameIds = g_manager->getAllFrames();
    std::string result;
    for (size_t i = 0; i < frameIds.size(); i++) {
        if (i > 0) result += ",";
        result += crdtIdToString(frameIds[i]);
    }
    
    strncpy(buffer, result.c_str(), bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
}

// Rectangle operations (using shapes)
EMSCRIPTEN_KEEPALIVE
const char* crdt_create_rectangle(const char* frameIdStr, double x, double y, double width, double height) {
    if (!g_manager) return nullptr;
    
    // Create rectangle as a CRDT node
    CRDTId rectId = g_manager->getDocument().createNode("rectangle");
    g_manager->getDocument().setNodeProperty(rectId, "x", std::to_string(x));
    g_manager->getDocument().setNodeProperty(rectId, "y", std::to_string(y));
    g_manager->getDocument().setNodeProperty(rectId, "width", std::to_string(width));
    g_manager->getDocument().setNodeProperty(rectId, "height", std::to_string(height));
    g_manager->getDocument().setNodeProperty(rectId, "fill", "#FFFFFF");
    
    // Add to root frame if no frame specified
    CRDTId rootId = g_manager->getDocument().getRootId();
    g_manager->getDocument().addChild(rootId, rectId);
    
    std::string idStr = crdtIdToString(rectId);
    char* result = (char*)malloc(idStr.length() + 1);
    strcpy(result, idStr.c_str());
    return result;
}

EMSCRIPTEN_KEEPALIVE
double crdt_rectangle_get_x(const char* rectIdStr) {
    if (!g_manager) return 0.0;
    CRDTId rectId = stringToCRDTId(rectIdStr);
    auto node = g_manager->getDocument().getNode(rectId);
    if (node && node->hasProperty("x")) {
        return std::stod(node->getProperty("x"));
    }
    return 0.0;
}

EMSCRIPTEN_KEEPALIVE
double crdt_rectangle_get_y(const char* rectIdStr) {
    if (!g_manager) return 0.0;
    CRDTId rectId = stringToCRDTId(rectIdStr);
    auto node = g_manager->getDocument().getNode(rectId);
    if (node && node->hasProperty("y")) {
        return std::stod(node->getProperty("y"));
    }
    return 0.0;
}

EMSCRIPTEN_KEEPALIVE
double crdt_rectangle_get_width(const char* rectIdStr) {
    if (!g_manager) return 0.0;
    CRDTId rectId = stringToCRDTId(rectIdStr);
    auto node = g_manager->getDocument().getNode(rectId);
    if (node && node->hasProperty("width")) {
        return std::stod(node->getProperty("width"));
    }
    return 0.0;
}

EMSCRIPTEN_KEEPALIVE
double crdt_rectangle_get_height(const char* rectIdStr) {
    if (!g_manager) return 0.0;
    CRDTId rectId = stringToCRDTId(rectIdStr);
    auto node = g_manager->getDocument().getNode(rectId);
    if (node && node->hasProperty("height")) {
        return std::stod(node->getProperty("height"));
    }
    return 0.0;
}

EMSCRIPTEN_KEEPALIVE
void crdt_rectangle_set_position(const char* rectIdStr, double x, double y) {
    if (!g_manager) return;
    CRDTId rectId = stringToCRDTId(rectIdStr);
    g_manager->getDocument().setNodeProperty(rectId, "x", std::to_string(x));
    g_manager->getDocument().setNodeProperty(rectId, "y", std::to_string(y));
}

EMSCRIPTEN_KEEPALIVE
void crdt_rectangle_set_size(const char* rectIdStr, double width, double height) {
    if (!g_manager) return;
    CRDTId rectId = stringToCRDTId(rectIdStr);
    g_manager->getDocument().setNodeProperty(rectId, "width", std::to_string(width));
    g_manager->getDocument().setNodeProperty(rectId, "height", std::to_string(height));
}

EMSCRIPTEN_KEEPALIVE
void crdt_rectangle_delete(const char* rectIdStr) {
    if (!g_manager) return;
    CRDTId rectId = stringToCRDTId(rectIdStr);
    g_manager->getDocument().deleteNode(rectId);
}

// Text box operations
EMSCRIPTEN_KEEPALIVE
const char* crdt_create_textbox(double x, double y, double width, double height, const char* text) {
    if (!g_manager) return nullptr;
    
    CRDTId textId = g_manager->getDocument().createNode("text");
    g_manager->getDocument().setNodeProperty(textId, "x", std::to_string(x));
    g_manager->getDocument().setNodeProperty(textId, "y", std::to_string(y));
    g_manager->getDocument().setNodeProperty(textId, "width", std::to_string(width));
    g_manager->getDocument().setNodeProperty(textId, "height", std::to_string(height));
    g_manager->getDocument().setNodeProperty(textId, "text", text ? std::string(text) : "");
    
    // Add to root frame
    CRDTId rootId = g_manager->getDocument().getRootId();
    g_manager->getDocument().addChild(rootId, textId);
    
    std::string idStr = crdtIdToString(textId);
    char* result = (char*)malloc(idStr.length() + 1);
    strcpy(result, idStr.c_str());
    return result;
}

EMSCRIPTEN_KEEPALIVE
void crdt_textbox_set_text(const char* textIdStr, const char* text) {
    if (!g_manager) return;
    CRDTId textId = stringToCRDTId(textIdStr);
    g_manager->getDocument().setNodeProperty(textId, "text", text ? std::string(text) : "");
}

EMSCRIPTEN_KEEPALIVE
const char* crdt_textbox_get_text(const char* textIdStr) {
    if (!g_manager) return nullptr;
    CRDTId textId = stringToCRDTId(textIdStr);
    auto node = g_manager->getDocument().getNode(textId);
    if (node && node->hasProperty("text")) {
        std::string text = node->getProperty("text");
        char* result = (char*)malloc(text.length() + 1);
        strcpy(result, text.c_str());
        return result;
    }
    return nullptr;
}

// Get all rectangles
EMSCRIPTEN_KEEPALIVE
void crdt_get_all_rectangles(char* buffer, int bufferSize) {
    if (!g_manager) {
        buffer[0] = '\0';
        return;
    }
    
    std::vector<std::string> rectIds;
    auto allNodes = g_manager->getDocument().getAllNodeIds();
    for (const auto& nodeId : allNodes) {
        auto node = g_manager->getDocument().getNode(nodeId);
        if (node && node->getType() == "rectangle" && !node->isDeleted()) {
            rectIds.push_back(crdtIdToString(nodeId));
        }
    }
    
    std::string result;
    for (size_t i = 0; i < rectIds.size(); i++) {
        if (i > 0) result += ",";
        result += rectIds[i];
    }
    
    strncpy(buffer, result.c_str(), bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
}

// Get all text boxes
EMSCRIPTEN_KEEPALIVE
void crdt_get_all_textboxes(char* buffer, int bufferSize) {
    if (!g_manager) {
        buffer[0] = '\0';
        return;
    }
    
    std::vector<std::string> textIds;
    auto allNodes = g_manager->getDocument().getAllNodeIds();
    for (const auto& nodeId : allNodes) {
        auto node = g_manager->getDocument().getNode(nodeId);
        if (node && node->getType() == "text" && !node->isDeleted()) {
            textIds.push_back(crdtIdToString(nodeId));
        }
    }
    
    std::string result;
    for (size_t i = 0; i < textIds.size(); i++) {
        if (i > 0) result += ",";
        result += textIds[i];
    }
    
    strncpy(buffer, result.c_str(), bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
}

// Free allocated string
EMSCRIPTEN_KEEPALIVE
void crdt_free_string(char* str) {
    if (str) {
        free(str);
    }
}

} // extern "C"

