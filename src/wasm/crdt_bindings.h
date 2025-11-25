#pragma once

#include <emscripten.h>
#include <string>
#include "../core/vector_crdt.h"
#include "../collaboration/crdt.h"

namespace Lienzo {

// Helper to convert CRDTId to/from string for WASM
inline std::string crdtIdToString(const CRDTId& id) {
    return id.toString();
}

inline CRDTId stringToCRDTId(const std::string& str) {
    size_t colonPos = str.find(':');
    if (colonPos != std::string::npos) {
        std::string siteId = str.substr(0, colonPos);
        uint64_t clock = std::stoull(str.substr(colonPos + 1));
        return CRDTId(siteId, clock);
    }
    return CRDTId();
}

} // namespace Lienzo

