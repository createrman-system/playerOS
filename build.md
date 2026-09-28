Manual Build (If You Prefer)
Step 1: Install Dependencies
bash

sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    qt5-qmake \
    qt5-default \
    libqt5gui5 \
    libqt5core5a \
    libqt5multimedia5 \
    libqt5multimediagsttools5 \
    libts-dev \
    tslib
Step 2: Create Build Directory
bash

cd /home/mypc/pilayer/MusicPlayerOS
mkdir build
cd build
Step 3: Configure with CMake
bash

cmake ..
Step 4: Compile
bash

make -j$(nproc)
Step 5: Install
bash

sudo make install
Alternative: Using Makefile
bash

cd /home/mypc/pilayer/MusicPlayerOS

# Build only
make build

# Build and install
make install

# View all options
make help
Build Options
Release Build (Optimized)
bash

cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
Debug Build (With symbols)
bash

cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
Clean Build
bash

rm -rf build
mkdir build && cd build
cmake ..
make -j$(nproc)
Verify Build
After building, check if it worked:

bash

# Check if binary exists
ls -la /usr/local/bin/MusicPlayerOS

# Run the app manually
/usr/local/bin/MusicPlayerOS

# Check systemd service
sudo systemctl status MusicPlayerOS
Enable Autostart
After building:

bash

sudo systemctl enable MusicPlayerOS
sudo systemctl start MusicPlayerOS
Or use Makefile:

bash

make enable-autostart
Troubleshooting Build Issues
CMake not found
bash

sudo apt-get install cmake
Qt5 libraries missing
bash

sudo apt-get install --reinstall qt5-default libqt5gui5-dev libqt5core5a-dev
Build fails with errors
bash

# Clean and retry
rm -rf build
mkdir build && cd build
cmake ..
make clean
make -j$(nproc)
Permission denied
bash

# Make build script executable
chmod +x build.sh

# Run with sudo if needed
sudo ./build.sh
Check Build Status
bash

# View detailed build output
cd build
make VERBOSE=1

# Check what will be installed
cd build
cmake -P cmake_install.cmake

# View service logs
journalctl -u MusicPlayerOS -f
