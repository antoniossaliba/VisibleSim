# macOS 15 Sequoia Compatibility Fixes for VisibleSim

## Summary

Successfully configured VisibleSim to run natively on macOS 15 Sequoia (Apple Silicon) without requiring XQuartz or X11. The application now uses macOS's native OpenGL/GLUT frameworks.

## Changes Made

### 1. Main Makefile (`/Makefile`)

**Updated library paths for macOS:**
- Changed from `/usr/local/lib` to Homebrew paths (`/opt/homebrew/lib`)
- Removed X11 dependencies (`/usr/X11/lib`, `libglut.dylib`)
- Updated to link against system GLUT framework instead of FreeGLUT

**Updated include paths:**
- Added platform-specific includes with GLEW from Homebrew
- `/opt/homebrew/opt/glew/include` for GLEW headers
- `/opt/homebrew/include` for other libraries

### 2. Application Makefile (`applicationsSrc/hexanodes_demo/Makefile`)

**Fixed standalone build support:**
- Added macOS-specific include paths when `GLOBAL_INCLUDES` is empty
- Added macOS-specific library paths when `GLOBAL_LIBS` is empty
- Removed FreeGLUT and X11 dependencies
- Updated to use system GLUT framework

### 3. GLUT Compatibility Layer (`simulatorCore/src/gui/glutCompat.h`)

**Created new compatibility header:**
- Provides FreeGLUT-like API using macOS system GLUT
- Implements missing functions:
  - `glutSetOption_compat()` - no-op replacement (not needed on macOS)
  - `glutLeaveMainLoop_compat()` - uses `exit(0)` to terminate
- Defines missing constants:
  - `GLUT_ACTION_ON_WINDOW_CLOSE`
  - `GLUT_ACTION_CONTINUE_EXECUTION`
  - `GLUT_KEY_DELETE`
- Ensures GLEW is included before GLUT (critical for macOS)

### 4. Source File Updates

**Updated to use glutCompat.h instead of FreeGLUT:**
- `simulatorCore/src/gui/openglViewer.cpp`
- `simulatorCore/src/events/cppScheduler.cpp`
- `simulatorCore/src/gui/shaders.h`
- `simulatorCore/src/gui/vertexArray.h`
- `simulatorCore/src/gui/camera.h`

**Fixed muParser include path:**
- `simulatorCore/src/csg/csgParser.h` - changed from `<muparser/muParser.h>` to `<muParser.h>`

**Fixed duplicate case value:**
- `simulatorCore/src/gui/openglViewer.cpp` - removed redundant `GLUT_KEY_DELETE` case (line 537)

### 5. CMakeLists.txt (for CMake builds)

**Updated GLEW configuration:**
- Set `GLEW_INCLUDE_DIRS` to `/opt/homebrew/opt/glew/include`
- Ensured system GLUT takes precedence over Homebrew includes

**Removed GLFW dependency (CRITICAL FIX):**
- GLFW and GLUT are mutually exclusive OpenGL context creators
- Removed `find_package(glfw3 REQUIRED)` from main CMakeLists.txt
- Removed `glfw` from link_libraries in both CMakeLists files
- This prevents conflicts between GLFW and GLUT initialization

**Files modified:**
- `/CMakeLists.txt` - Removed glfw3 package and glfw linking
- `/simulatorCore/CMakeLists.txt` - Removed glfw from target_link_libraries

## Technical Details

### Graphics Stack
- **OpenGL**: macOS native (deprecated but functional)
- **GLUT**: System framework (`/System/Library/Frameworks/GLUT.framework`)
- **GLEW**: Homebrew 2.2.0 (`/opt/homebrew/opt/glew`)
- **muParser**: Homebrew (`/opt/homebrew/include`)

### Build System
- **Compiler**: AppleClang 17.0.0
- **C++ Standard**: C++17
- **Architecture**: arm64 (Apple Silicon)
- **Deployment Target**: macOS 10.15+

### Key Compatibility Issues Resolved

1. **FreeGLUT vs System GLUT**: 
   - FreeGLUT requires X11 on macOS
   - System GLUT is native and doesn't require X11
   - Created compatibility shims for missing FreeGLUT functions

2. **Header Include Order**:
   - GLEW must be included before GLUT to avoid OpenGL header conflicts
   - Fixed in glutCompat.h to ensure correct order

3. **Library Paths**:
   - Old paths pointed to `/usr/local` (Intel Homebrew location)
   - Updated to `/opt/homebrew` (Apple Silicon Homebrew location)

4. **Missing Constants/Functions**:
   - macOS GLUT doesn't define `GLUT_KEY_DELETE`
   - macOS GLUT doesn't have `glutSetOption()` or `glutLeaveMainLoop()`
   - Provided compatibility implementations

## Building and Running

### Build with Makefile (RECOMMENDED - fully tested):
```bash
cd /Users/jbassil/Projects/VisibleSim
make clean
make -j8
```

### Build with CMake (EXPERIMENTAL):
```bash
cd /Users/jbassil/Projects/VisibleSim
rm -rf build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j8
```

**Note**: CMake builds are currently experiencing runtime issues. Use the Makefile build system for now.

### Run an application (without XQuartz):
```bash
cd applicationsBin/hexanodes_demo
./hexanodesDemo -c config.xml
```

### Verify native GLUT linkage:
```bash
otool -L applicationsBin/hexanodes_demo/hexanodesDemo | grep GLUT
# Should show: /System/Library/Frameworks/GLUT.framework/Versions/A/GLUT
```

## Testing Confirmation

Successfully tested on:
- **OS**: macOS 15 Sequoia  
- **Hardware**: Apple Silicon (arm64)
- **Application**: hexanodes_demo
- **Result**: GUI opens natively without XQuartz, shaders load correctly, simulation runs

## Notes

- XQuartz is no longer required
- The `DISPLAY` environment variable should NOT be set
- All OpenGL warnings about deprecation are expected (Apple deprecated OpenGL but it still works)
- CMake configuration is available but the traditional Makefile system is fully functional

## Future Considerations

If Apple removes OpenGL support entirely in future macOS versions, consider:
- Migrating to Metal via MoltenGL
- Using cross-platform alternatives like SDL2 + OpenGL
- Implementing a Metal renderer as an alternative backend
