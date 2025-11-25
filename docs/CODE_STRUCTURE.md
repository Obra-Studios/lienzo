# Code Structure Overview

## Architecture Layers

```
┌─────────────────────────────────────────────────────────────┐
│                    UI Layer (Web)                            │
│  examples/index.html - HTML/CSS/JavaScript UI               │
│  - Left sidebar (Pages, Layers, Frame properties, Plugins)  │
│  - Central canvas (rendering area)                         │
│  - Right sidebar (Chat interface)                           │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        │ JavaScript ↔ WASM Bridge
                        │ (via Emscripten bindings)
                        │
┌───────────────────────▼─────────────────────────────────────┐
│              WASM Bindings Layer                             │
│  src/wasm/main.cpp                                           │
│  - Exported C functions (EMSCRIPTEN_KEEPALIVE)              │
│  - JavaScript ↔ C++ interop                                 │
│  - Currently: create_canvas(), create_frame(), etc.         │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        │ C++ API calls
                        │
┌───────────────────────▼─────────────────────────────────────┐
│              Core Engine (C++)                               │
│                                                              │
│  ┌────────────────────────────────────────────────────┐    │
│  │  src/core/ - Vector Data Structures                │    │
│  │  - vector.h/cpp: Point, VectorPath, VectorShape    │    │
│  │  - frame.h/cpp: Frame containers                   │    │
│  │  - selection.h/cpp: Selection management           │    │
│  └────────────────────────────────────────────────────┘    │
│                                                              │
│  ┌────────────────────────────────────────────────────┐    │
│  │  src/canvas/ - Rendering                           │    │
│  │  - canvas.h/cpp: Canvas container                  │    │
│  │  - renderer.h/cpp: Rendering engine (TODO)         │    │
│  └────────────────────────────────────────────────────┘    │
│                                                              │
│  ┌────────────────────────────────────────────────────┐    │
│  │  src/collaboration/ - DOM Graph                    │    │
│  │  - dom_graph.h/cpp: Mergable DOM structure        │    │
│  │  - DOMNode: Represents design elements as nodes    │    │
│  │  - DOMGraph: Tree structure for collaboration      │    │
│  └────────────────────────────────────────────────────┘    │
│                                                              │
│  ┌────────────────────────────────────────────────────┐    │
│  │  src/plugins/ - Plugin System                      │    │
│  │  - plugin.h/cpp: IPlugin interface, PluginManager │    │
│  └────────────────────────────────────────────────────┘    │
│                                                              │
│  ┌────────────────────────────────────────────────────┐    │
│  │  src/ai/ - AI Integration                          │    │
│  │  - chat.h/cpp: ChatInterface for vector generation │    │
│  └────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────┘
```

## Where Things Live

### 1. **UI Layer** (`examples/index.html`)

The UI is a **pure web application** (HTML/CSS/JavaScript):
- **Location**: `examples/index.html`
- **Technology**: Vanilla HTML/CSS/JS (no framework currently)
- **Components**:
  - Left sidebar: Pages, Layers, Frame properties, Plugins
  - Central canvas: `<canvas id="canvas">` element
  - Right sidebar: Chat input interface
- **Status**: Static UI mockup, not yet connected to C++ code

### 2. **Vector Data Structures** (`src/core/`)

The actual vector data lives in **C++**:

#### `src/core/vector.h/cpp`
- **Point**: `{x, y}` coordinates
- **VectorPath**: Collection of points forming a path
- **VectorShape**: Abstract base class for all vector shapes
  - Subclasses would be: Rectangle, Ellipse, Path, etc. (to be implemented)

#### `src/core/frame.h/cpp`
- **Frame**: Container that holds vector shapes
- Each frame has position (x, y) and size (width, height)
- Frames can contain multiple `VectorShape` objects

#### `src/core/selection.h/cpp`
- **Selection**: Manages currently selected vector shapes
- Tracks which shapes are selected for operations

### 3. **CRDT System for Collaboration** (`src/collaboration/`)

The CRDT (Conflict-free Replicated Data Types) system enables real-time collaboration:

#### `src/collaboration/crdt.h/cpp`
- **CRDTId**: Unique identifier (`siteId:logicalClock`)
- **CRDTProperty<T>**: Last-Write-Wins property with timestamp
- **CRDTNode**: Base class for all design elements
  - Properties with LWW semantics
  - Children with tombstones for deletions
  - Merge operations for conflict-free sync
- **CRDTDocument**: Main document container
  - Manages all CRDT nodes
  - Provides merge operations
  - Handles serialization

#### `src/collaboration/dom_graph.h/cpp`
- Legacy DOM graph structure (kept for compatibility)
- Should be replaced with CRDT system in future

### 4. **CRDT-Aware Vector Structures** (`src/core/vector_crdt.h/cpp`)

Vector data structures integrated with CRDT:

#### `src/core/vector_crdt.h/cpp`
- **CRDTVectorShape**: Wraps `VectorShape` with CRDT properties
  - Syncs position, rotation, scale with CRDT nodes
  - All transformations generate CRDT operations
- **CRDTFrame**: CRDT-aware frame container
  - Position/size as CRDT properties
  - Children managed via CRDT
- **VectorCRDTManager**: High-level manager
  - Bridges vector shapes and CRDT document
  - Provides API for creating/editing
  - Handles merge operations

**Key Point**: The CRDT system is **integrated** with vector data structures:
- When you create/edit a `VectorShape`, it automatically updates CRDT nodes
- The CRDT document is what gets serialized and merged for collaboration
- Vector structures are synced from CRDT after merges

### 4. **Connection Layer** (`src/wasm/main.cpp`)

This is where **JavaScript calls C++**:

Currently exposes:
- `create_canvas()` - Creates a Canvas object
- `create_frame()` - Creates a Frame object  
- `add_frame_to_canvas()` - Adds frame to canvas

**Missing**: Most of the bridge code! You need:
- Functions to create vector shapes
- Functions to render to canvas
- Functions to handle mouse/keyboard input
- Functions to sync with DOM graph
- Functions to update UI when selection changes

## Data Flow (Current vs. Intended)

### Current State:
```
UI (HTML) ──X──> WASM ──> C++ Core
   (not connected)
```

### Intended Flow:
```
User clicks canvas
    ↓
JavaScript event handler (in index.html)
    ↓
Call WASM function (e.g., handle_click(x, y))
    ↓
C++ code (src/core/selection.cpp) - finds clicked shape
    ↓
C++ code (src/collaboration/dom_graph.cpp) - updates DOM graph
    ↓
C++ code (src/canvas/renderer.cpp) - re-renders
    ↓
WASM returns render data or calls JS callback
    ↓
JavaScript updates canvas and UI
```

## What's Missing

1. **JavaScript Bridge** (`examples/lienzo.js` or similar)
   - Wrapper around WASM module
   - Event handlers for canvas interactions
   - UI update functions
   - Connection to chat interface

2. **Rendering Implementation** (`src/canvas/renderer.cpp`)
   - Currently empty
   - Needs to render VectorShapes to WebGL or Canvas2D
   - Should expose render functions to WASM

3. **Input Handling** (`src/wasm/main.cpp`)
   - Mouse click handlers
   - Keyboard shortcuts
   - Drag operations

4. **DOM Graph Sync** 
   - Functions to convert VectorShape → DOMNode
   - Functions to sync changes between vector data and DOM graph
   - Serialization/deserialization

5. **UI Update Callbacks**
   - When selection changes → update left sidebar
   - When frame properties change → update property panel
   - When layers change → update layers list

## Recommended Next Steps

1. **Create JavaScript bridge** (`examples/lienzo-bridge.js`)
   - Load WASM module
   - Set up canvas event listeners
   - Create wrapper functions for WASM calls

2. **Implement renderer** (`src/canvas/renderer.cpp`)
   - Render frames and shapes
   - Expose render function to WASM

3. **Add input handlers** (`src/wasm/main.cpp`)
   - Mouse click → selection
   - Keyboard → shortcuts
   - Drag → transform

4. **Connect DOM graph**
   - Sync vector shapes with DOM nodes
   - Implement serialization for collaboration

