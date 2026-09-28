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
    echo -e "${YELLOW}Info: Not running on Raspberry Pi (development machine detected)${NC}"
fi

# Install required dependencies
echo -e "${GREEN}Installing dependencies...${NC}"
sudo apt-get update

# Try to install Qt5 packages - handle different package names across distros
echo -e "${GREEN}Installing Qt5 and build tools...${NC}"
sudo apt-get install -y \
    qt5-qmake \
    libqt5gui5 \
    libqt5core5a \
    libqt5multimedia5 \
    build-essential \
    cmake \
    git || true

# Install additional packages if available
sudo apt-get install -y \
    libqt5multimediagsttools5 \
    libts-dev \
    tslib || true

# Check if Qt5 is installed
if ! which qmake-qt5 >/dev/null 2>&1 && ! which qmake >/dev/null 2>&1; then
    echo -e "${YELLOW}Warning: qmake not found. Installing qt5-devel packages...${NC}"
    sudo apt-get install -y qtbase5-dev qtmultimedia5-dev || true
fi

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
