#include "music_player_app.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QPainter>
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>
#include <QListWidgetItem>
#include <QTimer>

MusicPlayerApp::MusicPlayerApp(QWidget* parent)
    : AppBase(parent),
      mediaPlayer(nullptr),
      audioProbe(nullptr),
      titleLabel(nullptr),
      artistLabel(nullptr),
      timeLabel(nullptr),
      durationLabel(nullptr),
      progressBar(nullptr),
      volumeSlider(nullptr),
      playlistWidget(nullptr),
      playButton(nullptr),
      pauseButton(nullptr),
      stopButton(nullptr),
      prevButton(nullptr),
      nextButton(nullptr),
      backButton(nullptr),
      currentTrackIndex(-1)
{
    setStyleSheet(
        "MusicPlayerApp { background-color: #0a0e27; }"
        "QLabel { color: white; background-color: transparent; }"
        "QPushButton { "
        "    background-color: #1a237e; "
        "    color: white; "
        "    border: 2px solid #3f51b5; "
        "    border-radius: 10px; "
        "    padding: 10px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "} "
        "QPushButton:pressed { "
        "    background-color: #0d1a5a; "
        "    border: 2px solid #5c6bc0; "
        "} "
        "QSlider::groove:horizontal { background-color: #2a2a2a; height: 8px; } "
        "QSlider::handle:horizontal { background-color: #00bcd4; width: 18px; } "
        "QListWidget { background-color: #1a1a2e; color: white; border: none; } "
        "QListWidget::item:selected { background-color: #3f51b5; } "
    );
    
    // Create media player
    mediaPlayer = new QMediaPlayer(this);
    audioProbe = new QAudioProbe(this);
    
    setupUI();
    connectSignals();
    loadMusicDirectory();
}

void MusicPlayerApp::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    
    // Top bar with back button
    QHBoxLayout* topBarLayout = new QHBoxLayout();
    backButton = new QPushButton("← Back", this);
    backButton->setMaximumWidth(100);
    backButton->setMaximumHeight(40);
    connect(backButton, &QPushButton::clicked, this, &MusicPlayerApp::onBack);
    topBarLayout->addWidget(backButton);
    topBarLayout->addStretch();
    mainLayout->addLayout(topBarLayout);
    
    // Now playing section
    QWidget* nowPlayingWidget = new QWidget(this);
    QVBoxLayout* nowPlayingLayout = new QVBoxLayout(nowPlayingWidget);
    nowPlayingLayout->setContentsMargins(0, 0, 0, 0);
    
    titleLabel = new QLabel("No track selected", this);
    titleLabel->setFont(QFont("Arial", 20, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    nowPlayingLayout->addWidget(titleLabel);
    
    artistLabel = new QLabel("", this);
    artistLabel->setFont(QFont("Arial", 14));
    artistLabel->setAlignment(Qt::AlignCenter);
    nowPlayingLayout->addWidget(artistLabel);
    
    mainLayout->addWidget(nowPlayingWidget);
    
    // Progress bar
    QHBoxLayout* progressLayout = new QHBoxLayout();
    timeLabel = new QLabel("0:00", this);
    timeLabel->setMaximumWidth(50);
    timeLabel->setFont(QFont("Arial", 12));
    progressLayout->addWidget(timeLabel);
    
    progressBar = new QProgressBar(this);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    progressBar->setStyleSheet(
        "QProgressBar { "
        "    border: 1px solid #3f51b5; "
        "    border-radius: 5px; "
        "    background-color: #1a1a2e; "
        "} "
        "QProgressBar::chunk { "
        "    background-color: #00bcd4; "
        "} "
    );
    progressLayout->addWidget(progressBar);
    
    durationLabel = new QLabel("0:00", this);
    durationLabel->setMaximumWidth(50);
    durationLabel->setFont(QFont("Arial", 12));
    progressLayout->addWidget(durationLabel);
    
    mainLayout->addLayout(progressLayout);
    
    // Volume control
    QHBoxLayout* volumeLayout = new QHBoxLayout();
    QLabel* volumeIconLabel = new QLabel("🔊", this);
    volumeIconLabel->setMaximumWidth(30);
    volumeLayout->addWidget(volumeIconLabel);
    
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setMinimum(0);
    volumeSlider->setMaximum(100);
    volumeSlider->setValue(70);
    volumeSlider->setMaximumWidth(200);
    connect(volumeSlider, QOverload<int>::of(&QSlider::valueChanged),
            this, &MusicPlayerApp::onVolumeChanged);
    volumeLayout->addWidget(volumeSlider);
    
    mainLayout->addLayout(volumeLayout);
    
    // Playback control buttons
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(10);
    
    prevButton = new QPushButton("⏮ Prev", this);
    connect(prevButton, &QPushButton::clicked, this, &MusicPlayerApp::onPrevious);
    controlLayout->addWidget(prevButton);
    
    playButton = new QPushButton("▶ Play", this);
    connect(playButton, &QPushButton::clicked, this, &MusicPlayerApp::onPlay);
    controlLayout->addWidget(playButton);
    
    pauseButton = new QPushButton("⏸ Pause", this);
    connect(pauseButton, &QPushButton::clicked, this, &MusicPlayerApp::onPause);
    controlLayout->addWidget(pauseButton);
    
    stopButton = new QPushButton("⏹ Stop", this);
    connect(stopButton, &QPushButton::clicked, this, &MusicPlayerApp::onStop);
    controlLayout->addWidget(stopButton);
    
    nextButton = new QPushButton("⏭ Next", this);
    connect(nextButton, &QPushButton::clicked, this, &MusicPlayerApp::onNext);
    controlLayout->addWidget(nextButton);
    
    mainLayout->addLayout(controlLayout);
    
    // Playlist
    QLabel* playlistTitle = new QLabel("Playlist:", this);
    playlistTitle->setFont(QFont("Arial", 14, QFont::Bold));
    mainLayout->addWidget(playlistTitle);
    
    playlistWidget = new QListWidget(this);
    playlistWidget->setMinimumHeight(150);
    connect(playlistWidget, &QListWidget::itemClicked,
            this, [this](QListWidgetItem* item) {
        int index = playlistWidget->row(item);
        currentTrackIndex = index;
        onPlaylistItemSelected(index);
    });
    mainLayout->addWidget(playlistWidget);
    
    setLayout(mainLayout);
}

void MusicPlayerApp::connectSignals() {
    connect(mediaPlayer, QOverload<QMediaPlayer::State>::of(&QMediaPlayer::stateChanged),
            this, [this](QMediaPlayer::State state) {
        playButton->setEnabled(state != QMediaPlayer::PlayingState);
        pauseButton->setEnabled(state == QMediaPlayer::PlayingState);
    });
    
    connect(mediaPlayer, &QMediaPlayer::positionChanged,
            this, &MusicPlayerApp::onProgressChanged);
    
    connect(mediaPlayer, &QMediaPlayer::durationChanged,
            this, &MusicPlayerApp::onDurationChanged);
    
    connect(mediaPlayer, QOverload<QMediaPlayer::MediaStatus>::of(&QMediaPlayer::mediaStatusChanged),
            this, &MusicPlayerApp::onMediaStatusChanged);
    
    // Setup audio probe for visualizations (if needed)
    audioProbe->setSource(mediaPlayer);
}

void MusicPlayerApp::loadMusicDirectory() {
    // Load music files from standard music directory
    QString musicPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    if (musicPath.isEmpty()) {
        musicPath = QDir::homePath() + "/Music";
    }
    
    QDir musicDir(musicPath);
    QStringList nameFilters;
    nameFilters << "*.mp3" << "*.wav" << "*.flac" << "*.aac" << "*.ogg" << "*.m4a";
    
    QFileInfoList files = musicDir.entryInfoList(nameFilters, QDir::Files);
    
    for (const auto& fileInfo : files) {
        playlist.append(fileInfo.absoluteFilePath());
        playlistWidget->addItem(fileInfo.baseName());
    }
    
    if (!playlist.isEmpty()) {
        playlistWidget->setCurrentRow(0);
        currentTrackIndex = 0;
        titleLabel->setText(QFileInfo(playlist[0]).baseName());
        qDebug() << "Loaded" << playlist.size() << "music files";
    } else {
        titleLabel->setText("No music files found");
        qDebug() << "No music files in" << musicPath;
    }
}

void MusicPlayerApp::onPlay() {
    if (currentTrackIndex >= 0 && currentTrackIndex < playlist.size()) {
        mediaPlayer->setMedia(QUrl::fromLocalFile(playlist[currentTrackIndex]));
        mediaPlayer->play();
        playButton->setEnabled(false);
        pauseButton->setEnabled(true);
    }
}

void MusicPlayerApp::onPause() {
    mediaPlayer->pause();
    playButton->setEnabled(true);
    pauseButton->setEnabled(false);
}

void MusicPlayerApp::onStop() {
    mediaPlayer->stop();
    playButton->setEnabled(true);
    pauseButton->setEnabled(false);
}

void MusicPlayerApp::onNext() {
    if (!playlist.isEmpty()) {
        currentTrackIndex = (currentTrackIndex + 1) % playlist.size();
        playlistWidget->setCurrentRow(currentTrackIndex);
        onPlay();
    }
}

void MusicPlayerApp::onPrevious() {
    if (!playlist.isEmpty()) {
        currentTrackIndex = (currentTrackIndex - 1 + playlist.size()) % playlist.size();
        playlistWidget->setCurrentRow(currentTrackIndex);
        onPlay();
    }
}

void MusicPlayerApp::onVolumeChanged(int value) {
    mediaPlayer->setVolume(value);
}

void MusicPlayerApp::onProgressChanged(qint64 position) {
    qint64 duration = mediaPlayer->duration();
    if (duration > 0) {
        progressBar->setValue((position * 100) / duration);
    }
    formatTime(position, timeLabel);
}

void MusicPlayerApp::onDurationChanged(qint64 duration) {
    formatTime(duration, durationLabel);
}

void MusicPlayerApp::onPlaylistItemSelected(int index) {
    currentTrackIndex = index;
    if (index >= 0 && index < playlist.size()) {
        titleLabel->setText(QFileInfo(playlist[index]).baseName());
        // Auto-play selected track
        QTimer::singleShot(100, this, &MusicPlayerApp::onPlay);
    }
}

void MusicPlayerApp::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        onNext();  // Auto-play next track
    }
}

void MusicPlayerApp::formatTime(qint64 ms, QLabel* label) {
    if (!label) return;
    
    qint64 seconds = ms / 1000;
    qint64 minutes = seconds / 60;
    seconds = seconds % 60;
    
    label->setText(QString::asprintf("%lld:%02lld", minutes, seconds));
}

void MusicPlayerApp::onBack() {
    emit requestHome();
}

void MusicPlayerApp::onAppActivated() {
    // Resume music player if needed
}

void MusicPlayerApp::onAppDeactivated() {
    // Optionally pause music when switching apps
    // mediaPlayer->pause();
}

void MusicPlayerApp::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(10, 14, 39));
    gradient.setColorAt(1, QColor(20, 30, 70));
    painter.fillRect(rect(), gradient);
    
    QWidget::paintEvent(event);
}
