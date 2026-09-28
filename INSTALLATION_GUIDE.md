# Music Player OS - Installation Guide for Raspberry Pi 3

## Prerequisites

### Hardware
- Raspberry Pi 3B or 3B+
- 3.5" Touch Display (Waveshare or compatible)
- MicroSD Card (16GB minimum recommended)
- USB Power Supply (2.5A)
- USB Keyboard and Mouse (for initial setup)
- HDMI Monitor (for initial setup)

### Software
- Raspberry Pi OS (Bullseye or newer)
- Internet connection for downloading packages

## Step-by-Step Installation

### 1. Prepare Raspberry Pi OS

#### Option A: Fresh Install
1. Download [Raspberry Pi OS](https://www.raspberrypi.com/software/)
2. Flash to MicroSD card using Raspberry Pi Imager
3. Select "Raspberry Pi OS (32-bit)" or "Raspberry Pi OS Lite (32-bit)"
4. Enable SSH during setup
5. Insert card into Raspberry Pi and power on

#### Option B: Existing Installation
Simply proceed with the next steps.

### 2. Initial System Setup

Connect via SSH or open a terminal on your Pi:

```bash
# Update system
sudo apt-get update
sudo apt-get upgrade -y

# Set timezone
sudo timedatectl set-timezone America/New_York  # Change as needed

# Enable SPI and I2C (if using additional sensors)
sudo raspi-config
# Navigate to: Interface Options → I2C/SPI → Enable
```

### 3. Install Display Driver

For Waveshare 3.5" touch display:

```bash
# Clone the driver repository
cd ~
git clone https://github.com/waveshare/LCD-show.git
cd LCD-show

# Install for 3.5" display
sudo chmod +x LCD35-show
sudo ./LCD35-show

# The Pi will reboot automatically
```

After reboot, verify display is working by checking HDMI output.

### 4. Configure Touch Input

```bash
# Install touch calibration tools
sudo apt-get install -y xserver-xorg-input-tslib libts-dev

# Create udev rule for touch device
sudo tee /etc/udev/rules.d/99-touchscreen.rules > /dev/null << EOF
SUBSYSTEM=="input", ATTRS{name}=="*", SYMLINK+="input/%k"
EOF

# Reload udev rules
sudo udevadm control --reload-rules
sudo udevadm trigger

# Verify touch device exists
ls -la /dev/input/
```

### 5. Disable Desktop Environment (Optional)

To run Music Player OS fullscreen without desktop:

```bash
# Switch to console
sudo systemctl set-default multi-user.target

# OR keep desktop but run as X11 app
sudo systemctl set-default graphical.target
```

### 6. Install Build Dependencies

```bash
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    qt5-qmake \
    qt5-default \
    libqt5gui5 \
    libqt5core5a \
    libqt5multimedia5 \
    libqt5multimediagsttools5 \
    libqt5dbus5 \
    libts-dev \
    tslib \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-alsa
```

### 7. Clone and Build Music Player OS

```bash
# Create application directory
mkdir -p ~/Projects
cd ~/Projects

# Clone the repository
git clone https://github.com/your-username/MusicPlayerOS.git
cd MusicPlayerOS

# Make build script executable
chmod +x build.sh

# Run build script
./build.sh
```

The build will take 10-15 minutes depending on your Pi's performance.

### 8. Verify Installation

```bash
# Check if binary was installed
ls -la /usr/local/bin/MusicPlayerOS

# Check systemd service
sudo systemctl status MusicPlayerOS

# Try running manually
/usr/local/bin/MusicPlayerOS
```

### 9. Configure Autostart

```bash
# Enable the service
sudo systemctl enable MusicPlayerOS

# Start the service
sudo systemctl start MusicPlayerOS

# View logs
journalctl -u MusicPlayerOS -f
```

### 10. Reboot and Test

```bash
sudo reboot
```

After reboot, your Music Player OS should start automatically on the touch display!

## Post-Installation Setup

### Create Music Directory

```bash
mkdir -p ~/Music
chmod 755 ~/Music
```

### Copy Music Files

Via SSH:
```bash
scp ~/Music/*.mp3 pi@raspberry-pi-ip:~/Music/
```

Via SCP or File Manager on another computer.

### Calibrate Touch Screen

1. Touch the Settings app
2. Tap "Calibrate Touch Screen"
3. Follow on-screen instructions
4. Tap each target point
5. Tap "Save"

## Configuration Files

After first run, configuration files are created at:

```
~/.config/MusicPlayerOS/
├── touch_calibration.json    # Touch calibration data
└── musicplayeros.log         # Application logs
```

## Systemd Service Management

```bash
# Start service
sudo systemctl start MusicPlayerOS

# Stop service
sudo systemctl stop MusicPlayerOS

# Restart service
sudo systemctl restart MusicPlayerOS

# Check status
sudo systemctl status MusicPlayerOS

# View logs
journalctl -u MusicPlayerOS -f

# View last 50 lines of logs
journalctl -u MusicPlayerOS -n 50

# Enable/disable autostart
sudo systemctl enable MusicPlayerOS
sudo systemctl disable MusicPlayerOS
```

## Troubleshooting Installation

### Build Fails
```bash
# Clean build directory
cd ~/Projects/MusicPlayerOS
rm -rf build

# Try again
./build.sh
```

### Qt5 Libraries Not Found
```bash
# Reinstall Qt5
sudo apt-get install --reinstall \
    qt5-default \
    libqt5gui5 \
    libqt5core5a \
    libqt5multimedia5

# Reconfigure
cd ~/Projects/MusicPlayerOS/build
cmake ..
make clean
make -j$(nproc)
```

### Touch Not Working
```bash
# Check touch device
cat /proc/bus/input/devices

# List input devices
ls -la /dev/input/

# Run calibration
/usr/local/bin/MusicPlayerOS
# Then: Settings → Calibrate Touch Screen
```

### Display Issues
```bash
# Check HDMI connection
tvservice -s

# Verify display config
cat /boot/config.txt | grep hdmi

# Rerun display setup
cd ~/LCD-show
sudo ./LCD35-show
```

### Application Won't Start
```bash
# Check permissions
sudo chown pi:pi /usr/local/bin/MusicPlayerOS
sudo chmod +x /usr/local/bin/MusicPlayerOS

# Run manually to see errors
/usr/local/bin/MusicPlayerOS

# Check systemd service
sudo systemctl status MusicPlayerOS
journalctl -u MusicPlayerOS -n 100
```

## Updating

To update to a newer version:

```bash
cd ~/Projects/MusicPlayerOS
git pull origin main

# Clean and rebuild
rm -rf build
./build.sh

# Restart service
sudo systemctl restart MusicPlayerOS
```

## Uninstallation

If you need to uninstall:

```bash
# Disable and stop service
sudo systemctl disable MusicPlayerOS
sudo systemctl stop MusicPlayerOS

# Remove service file
sudo rm /etc/systemd/system/MusicPlayerOS.service
sudo systemctl daemon-reload

# Remove binary
sudo rm /usr/local/bin/MusicPlayerOS

# Remove config (optional)
rm -rf ~/.config/MusicPlayerOS/

# Remove source (optional)
rm -rf ~/Projects/MusicPlayerOS
```

## Performance Optimization

```bash
# Disable WiFi if not needed (reduces power consumption)
sudo nmcli radio wifi off

# Disable Bluetooth if not needed
sudo systemctl disable hciuart.service

# Set GPU memory to minimum
sudo raspi-config
# Advanced Options → Memory Split → Set to 64MB

# Disable desktop environment completely
sudo systemctl set-default multi-user.target
```

## Monitoring

```bash
# Monitor system while running
watch -n 1 'free -h && echo "---" && vcgencmd measure_temp'

# Check CPU frequency
watch -n 1 'cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq'

# Monitor application memory usage
ps aux | grep MusicPlayerOS
```

---

**Installation complete!** Your Music Player OS is ready to use. 🎵

For troubleshooting, check the README.md file or review systemd logs.
