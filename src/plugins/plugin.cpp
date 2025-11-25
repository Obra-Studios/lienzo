#include "plugin.h"

namespace Lienzo {

PluginManager::PluginManager() {
}

void PluginManager::registerPlugin(std::shared_ptr<IPlugin> plugin) {
    if (plugin) {
        plugins[plugin->getName()] = plugin;
    }
}

void PluginManager::unregisterPlugin(const std::string& name) {
    plugins.erase(name);
}

std::shared_ptr<IPlugin> PluginManager::getPlugin(const std::string& name) {
    auto it = plugins.find(name);
    if (it != plugins.end()) {
        return it->second;
    }
    return nullptr;
}

void PluginManager::loadPlugin(const std::string& path) {
    // TODO: Implement plugin loading
}

void PluginManager::unloadPlugin(const std::string& name) {
    auto plugin = getPlugin(name);
    if (plugin) {
        plugin->deactivate();
        unregisterPlugin(name);
    }
}

} // namespace Lienzo

