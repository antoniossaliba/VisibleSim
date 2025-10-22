# VisibleSim - macOS Setup Guide

This document explains the changes made to make VisibleSim work on macOS 15 Sequoia (and newer versions) with Apple Silicon.

## Changes Made

### 1. CMakeLists.txt Updates

#### Main CMakeLists.txt
- Updated macOS deployment target from `11.0` to `15.0` to match the current SDK
- Configured proper paths for Homebrew packages (`/opt/homebrew`)
- Added FreeGLUT instead of system GLUT (which lacks required functions)
- Added proper framework linking for Cocoa, IOKit, and CoreVideo
- Added `GL_SILENCE_DEPRECATION` compile definition to suppress OpenGL deprecation warnings

#### simulatorCore/CMakeLists.txt
- Updated library linking to use FreeGLUT explicitly
- Added macOS frameworks required for GLFW

### 2. Configuration File Format

Updated XML configuration files to use the newer format:

**Old format (deprecated):**
```xml
<world gridSize="11,10,1" windowSize="1024,800">
```

**New format:**
```xml
<world gridSize="11,10,1">
    <visuals>
        <window size="1024,800"/>
    </visuals>
```

This eliminates the deprecation warning: "warning [DEPRECATED]: place windowSize in visuals tag!"

## Prerequisites

### Required Software

Install the following via Homebrew:

```bash
brew install glew
brew install glfw
brew install freeglut
brew install muparser
brew install cmake
```

### Check Installation

Verify all dependencies are installed:

```bash
brew list | grep -E "glew|glfw|freeglut|muparser"
```

## Building the Project

### Clean Build

```bash
# Remove old build files
rm -rf build

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build with parallel jobs
make -j$(sysctl -n hw.ncpu)
```

### Quick Rebuild

If you've already built once and just need to rebuild:

```bash
cd build
make -j$(sysctl -n hw.ncpu)
```

## Running Simulations

Navigate to the appropriate application binary directory and run:

```bash
cd applicationsBin/hexanodes_demo
./hexanodes_demo -c config.xml
```

Or use the convenient script:

```bash
./runBlockCode.sh hexanodes_demo
```

## Common Issues and Solutions

### Issue: X11/GLUT Errors

**Error:**
```
X Error of failed request:  BadValue (integer parameter out of range for operation)
  Major opcode of failed request:  129 (Apple-DRI)
```

**Solution:** This was caused by using macOS system GLUT. The CMakeLists now uses FreeGLUT which has full support for the required functions.

### Issue: Undefined symbols for architecture arm64

**Error:**
```
Undefined symbols for architecture arm64:
  "_glutLeaveMainLoop", referenced from:
  "_glutSetOption", referenced from:
```

**Solution:** System GLUT on macOS doesn't have these functions. FreeGLUT provides them.

### Issue: Library version warnings

**Warning:**
```
ld: warning: building for macOS-15.0, but linking with dylib which was built for newer version 26.0
```

**Solution:** These are harmless warnings. The libraries are forward-compatible and will work fine. You can ignore them.

## Technical Details

### Why FreeGLUT instead of system GLUT?

macOS's system GLUT is based on an older version and lacks several modern functions:
- `glutLeaveMainLoop()` - Required for proper application shutdown
- `glutSetOption()` - Required for window configuration
- Better compatibility with modern OpenGL applications

FreeGLUT is a more complete, actively maintained implementation.

### macOS Framework Dependencies

The project requires these macOS frameworks:
- **Cocoa**: For window management and UI
- **IOKit**: For input/output device handling
- **CoreVideo**: For display synchronization
- **OpenGL**: For 3D rendering (deprecated but still functional)

## Development Notes

- The project now compiles with minimal warnings (mostly about deprecated functions like `sprintf`)
- All robot libraries (BlinkyBlocks, Catoms3D, Hexanodes, etc.) compile successfully
- The build uses C++17 standard

## Version Information

- **macOS:** 15 Sequoia (works on 11.0+)
- **Architecture:** Apple Silicon (arm64)
- **CMake:** 3.16+
- **Compiler:** AppleClang 17.0.0

## Setup Script

A convenience setup script is included: `setup_macos.sh`

Run it to install all dependencies and get build instructions:

```bash
./setup_macos.sh
```
