# Quick Start Guide - Music Player OS

## 5-Minute Setup

### Step 1: Install on Raspberry Pi

SSH into your Raspberry Pi or open a terminal:

```bash
cd ~
git clone <repository-url> MusicPlayerOS
cd MusicPlayerOS
chmod +x build.sh
./build.sh
```

The script will automatically install all dependencies and build the application.

### Step 2: Enable Autostart

```bash
sudo systemctl enable MusicPlayerOS
```

### Step 3: Reboot

```bash
sudo reboot
```

After reboot, Music Player OS should start automatically on your 3.5" touch display!

## First Time Use

### Calibrate Touch Screen
1. Tap the **Settings** app (gear icon)
2. Tap **Calibrate Touch Screen**
3. Follow the instructions - tap the target circles
4. Tap **Save** when done

### Add Music Files

Place your music files in the Music directory:

```bash
# Via SSH
scp ~/Music/song.mp3 pi@raspberry-pi-ip:~/Music/

# Via File Explorer in the app
- Open File Explorer from home screen
- Navigate to ~/Music/
- Copy music files there
```

### Play Music

1. From home screen, tap **Music Player** (note icon)
2. Your music files will appear in the playlist
3. Tap any song to play
4. Use the control buttons to play/pause/skip

## Common Commands

### Start the application manually
```bash
/usr/local/bin/MusicPlayerOS
```

### View live logs
```bash
journalctl -u MusicPlayerOS -f
```

### Stop the service
```bash
sudo systemctl stop MusicPlayerOS
```

### Restart the service
```bash
sudo systemctl restart MusicPlayerOS
```

## Home Screen Apps

| App | Function |
|-----|----------|
| 🎵 Music Player | Play your music collection |
| 📁 File Explorer | Browse files on your Pi |
| ✋ Touch Calibrate | Calibrate touch screen accuracy |
| ⚙️ Settings | Adjust volume and system settings |
| ℹ️ System Info | View CPU temp, memory, disk usage |

## Tips & Tricks

1. **Custom Music Folder**: Edit `music_player_app.cpp` line 95 to change the default music directory
2. **Change Theme**: Edit the `setStyleSheet()` calls in any app to customize colors
3. **Add New App**: Create a new header file in `include/` and source in `src/`, then register it in `app.cpp`

## Troubleshooting

**Touch not responding?**
- Open Settings → Calibrate Touch Screen
- Recalibrate the display

**No music showing up?**
- Ensure music files are in `~/Music/`
- Check file format (MP3, WAV, FLAC, AAC, OGG, M4A supported)

**Application won't start?**
```bash
# Check dependencies
dpkg -l | grep qt5

# Install missing Qt5
sudo apt-get install -y qt5-default libqt5multimedia5

# Try running manually
/usr/local/bin/MusicPlayerOS
```

**Want to stop autostart?**
```bash
sudo systemctl disable MusicPlayerOS
```

## Need Help?

Check the full README.md for detailed documentation and troubleshooting guides.

---

**You're all set!** Enjoy your Music Player OS! 🎶
