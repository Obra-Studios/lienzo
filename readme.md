# Lienzo

A high-performance vector design tool built with C++ and WebAssembly, optimized for speed, collaboration, and AI-powered vector generation.

## Overview

Lienzo is a modern vector design tool that combines the power of native C++ performance with web accessibility. It features real-time collaboration, an extensible plugin system, and AI-assisted design generation.

## Key Features

- 🎨 **Vector Drawing** - Professional-grade vector editing similar to Figma and Adobe Illustrator
- 👥 **Real-Time Collaboration** - Simultaneous multi-user editing with conflict-free synchronization
- 🔌 **Plugin System** - Extensible architecture similar to VS Code for building custom features
- 🤖 **AI Vector Generation** - Chat interface for generating vector assets using natural language

## Architecture

- **Grey Infinite Canvas** - Neutral design environment with infinite scroll/zoom
- **Selectable Frames** - Container-based design with integrated chat for AI generation
- **Context-Aware UI** - Selection-based property panels and menus
- **Plugin Marketplace** - Install and manage plugins from the left sidebar

## Design Philosophy

1. **Vertical space is cheap. Horizontal space is precious** - UI optimized for horizontal canvas space
2. **Less menus** - Direct manipulation over nested menus
3. **Rearrangable sections** - Customizable layout like VS Code
4. **WYSIWYG** - No hidden views, all functionality visible and accessible

## Getting Started

*Coming soon - project is in early development*

## Technical Stack

- **Language**: C++
- **Target**: WebAssembly (WASM)
- **Build**: Emscripten

## Development

See [technical-spec.md](./technical-spec.md) for detailed technical specifications.

## License

*TBD*
