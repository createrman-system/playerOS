#!/bin/bash

# Music Player OS Build Script for Raspberry Pi 3
# This script builds and installs the Music Player OS

set -e  # Exit on error

echo "========================================="
echo "Music Player OS - Build Script"
echo "========================================="
echo ""

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if running on Raspberry Pi
if ! grep -q "Raspberry Pi" /proc/device-tree/model 2>/dev/null; then
    echo -e "${YELLOW}Warning: Not running on a Raspberry Pi. Build may not work correctly.${NC}"
fi

# Install required dependencies
echo -e "${GREEN}Installing dependencies...${NC}"
sudo apt-get update
sudo apt-get install -y \
    qt5-qmake \
    qt5-default \
    libqt5gui5 \
    libqt5core5a \
    libqt5multimedia5 \
    libqt5multimediagsttools5 \
    build-essential \
    cmake \
    git \
    libts-dev \
    tslib

# Create build directory
echo -e "${GREEN}Creating build directory...${NC}"
if [ -d "build" ]; then
    rm -rf build
fi
mkdir -p build
cd build

# Run CMake
echo -e "${GREEN}Running CMake...${NC}"
cmake ..

# Build the project
echo -e "${GREEN}Building Music Player OS...${NC}"
make -j$(nproc)

# Install the application
echo -e "${GREEN}Installing application...${NC}"
sudo make install

# Copy systemd service file
echo -e "${GREEN}Installing systemd service...${NC}"
sudo cp ../MusicPlayerOS.service /etc/systemd/system/
sudo systemctl daemon-reload

# Create configuration directory
echo -e "${GREEN}Creating configuration directory...${NC}"
sudo mkdir -p /etc/musicplayeros
sudo chown pi:pi /etc/musicplayeros

echo ""
echo -e "${GREEN}=========================================${NC}"
echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "${GREEN}=========================================${NC}"
echo ""
echo "To start the Music Player OS service:"
echo "  sudo systemctl start MusicPlayerOS"
echo ""
echo "To enable autostart on boot:"
echo "  sudo systemctl enable MusicPlayerOS"
echo ""
echo "To view logs:"
echo "  journalctl -u MusicPlayerOS -f"
echo ""
