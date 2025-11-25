# How to Run Lienzo WASM Version

## Quick Start

1. **Ensure WASM is built** (if not already):
   ```bash
   source emsdk/emsdk_env.sh
   make build
   ```

2. **Start the development server** from the project root:
   ```bash
   npm run dev
   # or
   python3 -m http.server 8000
   ```

3. **Open in your browser**:
   ```
   http://localhost:8000/examples/index.html
   ```
   
   **Important**: Use `http://` (not `https://`) - the simple HTTP server doesn't support SSL.

## Verify WASM is Loading

1. Open browser DevTools (F12 or Cmd+Option+I)
2. Check the Console tab
3. You should see: `"Lienzo CRDT initialized with WASM"`
4. If you see errors, check:
   - WASM files exist in `build/` directory
   - Server is running from project root (not examples directory)
   - Browser console for specific error messages

## Troubleshooting

### WASM not loading
- Check that `build/lienzo.js` and `build/lienzo.wasm` exist
- Verify the server is running from the project root
- Check browser console for CORS or 404 errors

### Tool buttons not working
- Check browser console for JavaScript errors
- Verify the page loaded completely
- Try refreshing the page

### CRDT operations failing
- Check browser console for WASM function call errors
- Verify `crdt` object is initialized (check console)
- Try creating a simple frame to test

## Development Workflow

1. **Edit C++ code** in `src/`
2. **Rebuild WASM**:
   ```bash
   source emsdk/emsdk_env.sh
   make build
   ```
3. **Refresh browser** (hard refresh: Cmd+Shift+R or Ctrl+Shift+R)

## File Structure

```
lienzo/
├── build/              # WASM files (lienzo.js, lienzo.wasm)
├── examples/
│   ├── index.html      # Main UI
│   ├── lienzo-crdt.js   # JavaScript bridge
│   └── styles.css      # Styling
└── src/                # C++ source code
```

The HTML file imports WASM from `../build/lienzo.js`, so the server must run from the project root.

