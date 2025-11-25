# Lienzo Architecture

## Overview

Lienzo is architected as a modular C++ application compiled to WebAssembly, designed for high performance vector graphics editing with real-time collaboration.

## Core Modules

### 1. Core (`src/core/`)
Foundation for vector graphics operations:
- **Vector**: Basic vector primitives (Point, VectorPath, VectorShape)
- **Selection**: Selection management for design elements
- **Frame**: Container system for organizing design elements

### 2. Canvas (`src/canvas/`)
Rendering and viewport management:
- **Canvas**: Main canvas container managing frames and viewport
- **Renderer**: Rendering engine (to be implemented with WebGL/Canvas)

### 3. Collaboration (`src/collaboration/`)
Real-time multi-user synchronization:
- **DOMGraph**: Mergable DOM graph structure for conflict-free collaboration
- Operational transformation or CRDT-based merge algorithms

### 4. Plugins (`src/plugins/`)
Extensible plugin system:
- **IPlugin**: Plugin interface
- **PluginManager**: Plugin lifecycle and registration

### 5. AI (`src/ai/`)
AI-powered vector generation:
- **ChatInterface**: Chat UI integration for natural language to vector conversion

### 6. WASM (`src/wasm/`)
WebAssembly bindings and entry point:
- **main.cpp**: Exported C functions for JavaScript interop

## Data Flow

```
User Input → Canvas → Selection → Frame → Vector Shapes
                ↓
         Collaboration Layer (DOMGraph)
                ↓
         Renderer → WebGL/Canvas
```

## Collaboration Model

Uses a mergable DOM graph structure similar to Figma:
- Each design element is a node in the graph
- Operations are transformed and merged
- Conflict resolution through operational transformation or CRDTs

## Plugin System

Similar to VS Code's extension model:
- Plugins implement `IPlugin` interface
- PluginManager handles loading, activation, and lifecycle
- Plugins can extend UI, add tools, or modify behavior

## Performance Considerations

- WebAssembly for near-native performance
- Efficient memory management for large canvases
- Incremental rendering for smooth interactions
- Optimized collaboration message passing

