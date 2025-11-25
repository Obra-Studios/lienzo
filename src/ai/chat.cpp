#include "chat.h"

namespace Lienzo {

ChatInterface::ChatInterface() {
}

void ChatInterface::setFrame(std::shared_ptr<Frame> frame) {
    associatedFrame = frame;
}

void ChatInterface::sendMessage(const std::string& message) {
    processMessage(message);
}

void ChatInterface::setVectorGeneratedCallback(VectorGeneratedCallback callback) {
    onVectorGenerated = callback;
}

void ChatInterface::processMessage(const std::string& message) {
    // TODO: Parse message and determine intent
    // For now, treat all messages as vector generation requests
    generateVector(message);
}

void ChatInterface::generateVector(const std::string& prompt) {
    // TODO: Integrate with AI model (local or cloud)
    // For now, just call the callback with empty data
    if (onVectorGenerated) {
        onVectorGenerated("");
    }
}

} // namespace Lienzo

