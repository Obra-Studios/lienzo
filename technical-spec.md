# Lienzo Technical Specification

## Overview

Lienzo is a vector design tool created in C++ and compiled to WebAssembly. It is optimized for speed, collaboration, sharing, and AI vector generation.

## Architecture

### Core Components

1. **Grey Infinite Canvas**
   - Infinite scrollable/zoomable canvas
   - Grey background for neutral design environment
   - High-performance rendering optimized for WebAssembly

2. **Selectable Frames**
   - Frames act as containers for design elements
   - Each frame can have an associated chat interface
   - Chat interface enables AI-powered vector asset generation within frames
   - Frames are selectable and have properties panels

3. **Item Selection System**
   - Selecting items (frames, vectors, shapes) opens related menus on the left sidebar
   - Context-aware property panels based on selection
   - Multi-selection support

4. **Left Sidebar Menu**
   - Contains plugin section for installing various plugins
   - Dynamic plugin interface similar to VS Code
   - Rearrangable sections (like VS Code)
   - Context-sensitive panels based on selection

## Features

### 1. Vector Drawing
- Similar functionality to Figma and Adobe Illustrator
- Core vector manipulation tools:
  - Pen tool
  - Shape tools (rectangle, ellipse, polygon)
  - Path editing
  - Boolean operations
  - Transform tools

### 2. Simultaneous Multi-Collaboration
- Implemented through mergable DOM graph structures similar to Figma
- Real-time synchronization of design state
- Conflict resolution through operational transformation or CRDTs
- Network-efficient updates

### 3. Dynamic Plugin Interface
- Extensible architecture similar to VS Code
- Plugin API for building new features
- Plugin marketplace/installation system
- Hot-reloading of plugins

### 4. Chat Interface for Vector Asset Generation
- AI-powered vector generation within frames
- Natural language to vector conversion
- Context-aware generation based on frame properties
- Integration with AI models (local or cloud-based)

## Design Philosophy

1. **Vertical space is cheap. Horizontal space is precious**
   - UI elements should stack vertically when possible
   - Sidebars should be narrow and efficient
   - Maximize horizontal canvas space

2. **Less menus**
   - Reduce menu nesting
   - Prefer direct manipulation
   - Context-sensitive actions

3. **Rearrangable sections like VS Code**
   - Panels can be moved and docked
   - Customizable layout
   - Persistent layout preferences

4. **What you see is what you get**
   - No hidden views accessible through convoluted menus
   - All functionality accessible through visible UI
   - Clear visual hierarchy

## Technical Stack

- **Core Language**: C++
- **Compilation Target**: WebAssembly (WASM)
- **Build System**: TBD (CMake, Emscripten, etc.)
- **Rendering**: TBD (WebGL, Canvas API, or custom renderer)
- **UI Framework**: TBD (Custom or framework integration)

## Project Structure

```
lienzo/
├── src/
│   ├── core/           # Core vector engine
│   ├── canvas/          # Canvas rendering
│   ├── collaboration/   # Multi-user sync
│   ├── plugins/         # Plugin system
│   ├── ai/              # AI integration
│   └── wasm/            # WebAssembly bindings
├── plugins/             # Plugin examples
├── tests/               # Unit and integration tests
├── docs/                # Documentation
└── build/               # Build artifacts
```

## Implementation Phases

### Phase 1: Core Engine
- Basic vector drawing primitives
- Canvas rendering
- Selection system
- Frame system

### Phase 2: Collaboration
- DOM graph structure
- Merge algorithms
- Network layer
- Conflict resolution

### Phase 3: Plugin System
- Plugin API
- Plugin loader
- Plugin marketplace
- Example plugins

### Phase 4: AI Integration
- Chat interface
- AI model integration
- Vector generation pipeline
- Frame-based generation

## Performance Targets

- 60 FPS rendering at 4K resolution
- Sub-100ms latency for collaboration updates
- < 5MB initial WASM bundle size
- Efficient memory management for large canvases

