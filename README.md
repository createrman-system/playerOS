# Music Player OS for Raspberry Pi 3

A beautiful, touch-optimized mini operating system for Raspberry Pi 3 with a 3.5" touch display. This is a complete music player OS with app launcher, file explorer, system info, and touch screen calibration tool.

## Features

- **Music Player**: Full-featured music player with playlist support, play/pause/stop controls, volume control, and auto-play next track
- **Touch Calibrator**: 5-point touch screen calibration tool to ensure accurate touch input on your 3.5" display
- **File Explorer**: Navigate and browse files on your Raspberry Pi
- **Settings**: Audio volume control and access to touch calibration
- **System Info**: Real-time CPU temperature, memory usage, disk space, and uptime
- **Home Screen**: Beautiful app launcher with clock and date display
- **Fullscreen GUI**: Replaces desktop environment, starts on boot

## Hardware Requirements

- **Raspberry Pi 3** (or compatible)
- **3.5" Touch Display** (320x480 or similar resolution)
- **MicroSD Card** with Raspberry Pi OS installed
- **Power Supply** (2.5A recommended)
- **USB Hub** (if needed for peripherals)

## Installation

### 1. Prerequisites

Update your Raspberry Pi OS:
```bash
sudo apt-get update
sudo apt-get upgrade -y
```

### 2. Clone or Download the Project

```bash
cd ~
git clone <repository-url> MusicPlayerOS
cd MusicPlayerOS
```

### 3. Build and Install

Make the build script executable and run it:

```bash
chmod +x build.sh
./build.sh
```

This will:
- Install all required Qt5 and build dependencies
- Build the application from source
- Install to `/usr/local/bin/MusicPlayerOS`
- Install systemd service for autostart

### 4. Configure Display (if needed)

Edit `/boot/config.txt`:
```bash
sudo nano /boot/config.txt
```

Add these lines for your 3.5" touch display:
```
dtoverlay=waveshare35a
hdmi_group=2
hdmi_mode=1
hdmi_mode=87
hdmi_cvt 320 480 60 6 0 0 0
```

### 5. Configure Touch Input

For Waveshare 3.5" touch display, the touch device should be auto-detected. If needed, you can specify:
```bash
export QT_QPA_GENERIC_PLUGINS=tslib:/dev/input/event0
```

## Usage

### Start Manually

```bash
/usr/local/bin/MusicPlayerOS
```

### Enable Autostart on Boot

```bash
sudo systemctl enable MusicPlayerOS
sudo systemctl start MusicPlayerOS
```

### View Logs

```bash
journalctl -u MusicPlayerOS -f
```

### Stop the Service

```bash
sudo systemctl stop MusicPlayerOS
```

## Music Files

Place music files in your home `Music` directory:

```bash
~/Music/
```

Supported formats:
- MP3
- WAV
- FLAC
- AAC
- OGG
- M4A

## Touch Screen Calibration

1. Open the Settings app from the home screen
2. Click "Calibrate Touch Screen"
3. Follow the on-screen instructions
4. Tap the target circle 5 times (4 corners + center)
5. Click "Save" to save calibration

Calibration data is saved to: `~/.config/MusicPlayerOS/touch_calibration.json`

## Project Structure

```
MusicPlayerOS/
├── include/           # Header files
│   ├── app.h
│   ├── home_screen.h
│   ├── music_player_app.h
│   ├── touch_calibrator_app.h
│   ├── settings_app.h
│   ├── file_explorer_app.h
│   ├── sysinfo_app.h
│   ├── system.h
│   └── keyboard_ui.h
├── src/              # Source files
│   ├── main.cpp
│   ├── app.cpp
│   ├── home_screen.cpp
│   ├── music_player_app.cpp
│   ├── touch_calibrator_app.cpp
│   ├── settings_app.cpp
│   ├── file_explorer_app.cpp
│   ├── sysinfo_app.cpp
│   └── system.cpp
├── CMakeLists.txt    # Build configuration
├── build.sh          # Build script
├── MusicPlayerOS.service  # Systemd service
└── README.md         # This file
```

## Building from Scratch

If you want to build manually:

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
sudo make install
```

## Troubleshooting

### Touch not working
- Ensure your touch device is properly connected
- Check `/dev/input/` for the touch device (usually `event0` or `touchscreen0`)
- Run the Touch Calibrator from Settings
- Check kernel logs: `dmesg | tail -20`

### Display shows nothing
- Verify display is connected and powered
- Check `/boot/config.txt` for display settings
- Reboot after changing display configuration

### Music not playing
- Verify music files are in `~/Music/` directory
- Check file formats are supported (MP3, WAV, FLAC, etc.)
- Check system volume is not muted (Settings app)
- Check journalctl logs for errors

### Application won't start
- Check if Qt5 libraries are installed: `dpkg -l | grep qt5`
- Verify permissions: `ls -la /usr/local/bin/MusicPlayerOS`
- Check systemd logs: `journalctl -u MusicPlayerOS -n 50`

## Performance Tips

- Disable unnecessary services on your Raspberry Pi
- Use a fast MicroSD card (Class 10 or higher recommended)
- Keep music files on fast storage (external SSD if possible)
- Close unnecessary browser tabs and applications

## Development

To modify and rebuild:

```bash
# Edit source files in include/ and src/
cd build
cmake ..
make
sudo make install
```

## License

This project is provided as-is for use on Raspberry Pi systems.

## Support

For issues, questions, or feature requests:
1. Check the troubleshooting section above
2. Review systemd logs: `journalctl -u MusicPlayerOS -f`
3. Check application logs in `~/.config/MusicPlayerOS/`

## Version History

- **v1.0** - Initial release with music player, file explorer, settings, and touch calibrator

## Credits

Built with:
- Qt5 - Cross-platform application framework
- C++ - Programming language
- CMake - Build system
- Raspberry Pi OS - Linux distribution

---

**Enjoy your Music Player OS!** 🎵
