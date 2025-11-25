# Deploying Lienzo to Vercel

This guide will help you deploy Lienzo to Vercel.

## Prerequisites

1. **Vercel account**: Sign up at [vercel.com](https://vercel.com) if you don't have one
2. **Vercel CLI** (optional but recommended):
   ```bash
   npm i -g vercel
   ```

## Pre-Deployment Setup

### Option 1: Pre-build WASM files (Recommended)

Since Emscripten SDK is large and may not be available in Vercel's build environment, it's recommended to pre-build the WASM files locally and commit them:

```bash
# 1. Activate Emscripten SDK
source emsdk/emsdk_env.sh

# 2. Build WASM files
make build

# 3. Verify build files exist
ls -la build/lienzo.js build/lienzo.wasm

# 4. Commit the build files
git add build/lienzo.js build/lienzo.wasm
git commit -m "Add WASM build files for deployment"
```

### Option 2: Build during deployment

The build script will attempt to build WASM files during Vercel deployment if they're not found. However, this requires Emscripten SDK to be available in the build environment, which may not always work.

## Deployment Steps

### Using Vercel CLI

1. **Login to Vercel**:
   ```bash
   vercel login
   ```

2. **Deploy**:
   ```bash
   vercel
   ```
   
   Follow the prompts:
   - Link to existing project? (Choose based on your preference)
   - Project name: `lienzo` (or your preferred name)
   - Directory: `.` (current directory)

3. **Deploy to production**:
   ```bash
   vercel --prod
   ```

### Using Vercel Dashboard

1. Go to [vercel.com/new](https://vercel.com/new)
2. Import your Git repository (GitHub, GitLab, or Bitbucket)
3. Vercel will automatically detect the configuration
4. Click "Deploy"

## Configuration Files

The following files have been set up for Vercel deployment:

- **`vercel.json`**: Vercel configuration with routing and headers
- **`scripts/build-vercel.js`**: Build script that checks for WASM files
- **`package.json`**: Updated with `build:vercel` script

## What Gets Deployed

- `examples/index.html` - Main application (served at root `/`)
- `examples/lienzo-crdt.js` - JavaScript bridge
- `examples/styles.css` - Styles
- `build/lienzo.js` - WASM JavaScript loader
- `build/lienzo.wasm` - WebAssembly binary

## Troubleshooting

### WASM files not found

If you see errors about missing WASM files:

1. **Pre-build locally** (recommended):
   ```bash
   source emsdk/emsdk_env.sh
   make build
   git add build/
   git commit -m "Add WASM build files"
   git push
   ```

2. **Check build logs**: In Vercel dashboard, check the build logs to see if the build script ran successfully.

### CORS or WASM loading errors

The `vercel.json` includes proper headers for WASM files. If you still see CORS errors:

1. Check that `vercel.json` is committed
2. Verify the headers configuration in Vercel dashboard
3. Check browser console for specific error messages

### Build fails during deployment

If the build fails because Emscripten is not available:

1. Pre-build WASM files locally (see Option 1 above)
2. Commit and push the build files
3. Redeploy

## Environment Variables

Currently, no environment variables are required. If you need to add any in the future:

1. Go to Vercel dashboard → Your project → Settings → Environment Variables
2. Add variables for Production, Preview, and Development as needed

## Custom Domain

To add a custom domain:

1. Go to Vercel dashboard → Your project → Settings → Domains
2. Add your domain
3. Follow DNS configuration instructions

## Continuous Deployment

Once connected to a Git repository, Vercel will automatically deploy:
- **Production**: On pushes to your main branch
- **Preview**: On pushes to other branches and pull requests

## Next Steps

After deployment:

1. Visit your deployment URL (provided by Vercel)
2. Test the application functionality
3. Check browser console for any errors
4. Verify WASM is loading correctly (should see "Lienzo CRDT initialized with WASM" in console)

