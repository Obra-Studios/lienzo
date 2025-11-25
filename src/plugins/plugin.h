#pragma once

#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

namespace Lienzo {

// Plugin interface similar to VS Code
class IPlugin {
public:
    virtual ~IPlugin() = default;
    
    virtual std::string getName() const = 0;
    virtual std::string getVersion() const = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
};

class PluginManager {
public:
    PluginManager();
    
    void registerPlugin(std::shared_ptr<IPlugin> plugin);
    void unregisterPlugin(const std::string& name);
    std::shared_ptr<IPlugin> getPlugin(const std::string& name);
    
    void loadPlugin(const std::string& path);
    void unloadPlugin(const std::string& name);
    
private:
    std::unordered_map<std::string, std::shared_ptr<IPlugin>> plugins;
};

} // namespace Lienzo

