#!/usr/bin/env node

/**
 * Vercel build script for Lienzo
 * 
 * This script:
 * 1. Checks if WASM files are already built
 * 2. If not, attempts to build them using Emscripten (if available)
 * 3. Provides clear error messages if building is not possible
 */

const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

const BUILD_DIR = path.join(__dirname, '..', 'build');
const WASM_JS = path.join(BUILD_DIR, 'lienzo.js');
const WASM_BINARY = path.join(BUILD_DIR, 'lienzo.wasm');

function checkWasmFiles() {
  return fs.existsSync(WASM_JS) && fs.existsSync(WASM_BINARY);
}

function tryBuild() {
  console.log('WASM files not found. Attempting to build...');
  
  // Check if Emscripten is available
  const emsdkPath = path.join(__dirname, '..', 'emsdk', 'emsdk_env.sh');
  
  if (!fs.existsSync(emsdkPath)) {
    console.error('❌ Error: Emscripten SDK not found at expected location.');
    console.error('   Please ensure emsdk is installed in the emsdk/ directory.');
    console.error('   Or pre-build WASM files and commit them to the build/ directory.');
    process.exit(1);
  }

  try {
    console.log('Activating Emscripten SDK...');
    // Source emsdk_env.sh and run make build
    // Note: This might not work perfectly in all environments
    // In that case, pre-build WASM files locally and commit them
    execSync('bash -c "source emsdk/emsdk_env.sh && make build"', {
      cwd: path.join(__dirname, '..'),
      stdio: 'inherit'
    });
    
    if (checkWasmFiles()) {
      console.log('✅ WASM files built successfully!');
      return true;
    } else {
      console.error('❌ Build completed but WASM files not found.');
      process.exit(1);
    }
  } catch (error) {
    console.error('❌ Build failed:', error.message);
    console.error('\n💡 Tip: Pre-build WASM files locally and commit them:');
    console.error('   1. Run: source emsdk/emsdk_env.sh');
    console.error('   2. Run: make build');
    console.error('   3. Commit the build/ directory');
    process.exit(1);
  }
}

// Main execution
console.log('🔍 Checking for WASM files...');

if (checkWasmFiles()) {
  console.log('✅ WASM files found. Ready for deployment!');
  process.exit(0);
} else {
  console.log('⚠️  WASM files not found.');
  tryBuild();
}

