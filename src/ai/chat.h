#pragma once

#include <string>
#include <functional>
#include "../core/frame.h"

namespace Lienzo {

// Chat interface for AI vector generation
class ChatInterface {
public:
    ChatInterface();
    
    void setFrame(std::shared_ptr<Frame> frame);
    void sendMessage(const std::string& message);
    
    // Callback for when AI generates a vector
    using VectorGeneratedCallback = std::function<void(const std::string& vectorData)>;
    void setVectorGeneratedCallback(VectorGeneratedCallback callback);
    
private:
    std::shared_ptr<Frame> associatedFrame;
    VectorGeneratedCallback onVectorGenerated;
    
    void processMessage(const std::string& message);
    void generateVector(const std::string& prompt);
};

} // namespace Lienzo

