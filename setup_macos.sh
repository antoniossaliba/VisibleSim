#!/bin/bash
# Setup script for VisibleSim on macOS 15 (Sequoia)

echo "Setting up VisibleSim for macOS..."

# Check if Homebrew is installed
if ! command -v brew &> /dev/null; then
    echo "Homebrew not found. Please install Homebrew first:"
    echo '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
    exit 1
fi

echo "Installing dependencies via Homebrew..."

# Install required packages
brew install glew
brew install glfw
brew install freeglut
brew install muparser
brew install cmake

echo "Dependencies installed successfully!"
echo ""
echo "Now you can build the project:"
echo "1. Clean the build directory:"
echo "   rm -rf build"
echo ""
echo "2. Configure with CMake:"
echo "   cmake -B build -S ."
echo ""
echo "3. Build the project:"
echo "   cmake --build build"
echo ""
echo "Or use the traditional approach:"
echo "   cd build"
echo "   cmake .."
echo "   make -j$(sysctl -n hw.ncpu)"
