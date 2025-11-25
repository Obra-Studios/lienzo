# Development Guide

## Prerequisites

1. **Emscripten SDK** - Required for compiling C++ to WebAssembly
   ```bash
   # Install Emscripten
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```

2. **CMake** (optional, for CMake-based builds)
   ```bash
   brew install cmake  # macOS
   # or
   apt-get install cmake  # Linux
   ```

3. **Make** - Usually pre-installed on Unix systems

## Building

### Using Makefile

```bash
make build
```

This will compile all C++ sources and generate:
- `build/lienzo.js` - JavaScript glue code
- `build/lienzo.wasm` - WebAssembly binary

### Using CMake

```bash
mkdir build
cd build
emcmake cmake ..
emmake make
```

## Running

1. Start a local web server from the project root:
   ```bash
   npm run serve
   # or
   python3 -m http.server 8000
   ```

2. Open the page in your browser using **HTTP** (not HTTPS):
   ```
   http://localhost:8000/examples/index.html
   ```
   
   **Important**: Make sure you use `http://` not `https://` - the simple HTTP server doesn't support SSL/TLS.

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
├── examples/            # Example HTML/JS
└── build/               # Build artifacts
```

## Development Workflow

1. **Edit C++ source files** in `src/`
2. **Rebuild** with `make build` or `make`
3. **Refresh browser** to see changes
4. **Test** in browser DevTools console

## Adding New Features

### Adding a New Vector Shape

1. Create a new class inheriting from `VectorShape` in `src/core/`
2. Implement `getPath()` and `transform()` methods
3. Add to selection system

### Adding a Plugin

1. Create plugin class implementing `IPlugin` interface
2. Register with `PluginManager`
3. Add UI integration in left sidebar

### Adding Collaboration Features

1. Extend `DOMGraph` with new node types
2. Implement operation transformation logic
3. Add network layer for real-time sync

## Debugging

- Use browser DevTools to debug JavaScript/WASM interop
- Enable source maps in Emscripten build flags
- Use `console.log` in C++ via `EM_ASM` macro
- Use Emscripten's debugging tools

## Testing

*Test framework to be determined*

## Performance Profiling

- Use browser Performance tab
- Profile WASM execution
- Monitor memory usage
- Optimize hot paths

