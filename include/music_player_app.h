#ifndef MUSIC_PLAYER_APP_H
#define MUSIC_PLAYER_APP_H

#include "app.h"
#include <QMediaPlayer>
#include <QAudioProbe>
#include <QSlider>
#include <QLabel>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>

class MusicPlayerApp : public AppBase {
    Q_OBJECT

public:
    explicit MusicPlayerApp(QWidget* parent = nullptr);
    QString getAppName() const override { return "MusicPlayer"; }
    void onAppActivated() override;
    void onAppDeactivated() override;

private:
    void setupUI();
    void createPlaylist();
    void connectSignals();
    void loadMusicDirectory();

    QMediaPlayer* mediaPlayer;
    QAudioProbe* audioProbe;
    
    QLabel* titleLabel;
    QLabel* artistLabel;
    QLabel* timeLabel;
    QLabel* durationLabel;
    QProgressBar* progressBar;
    QSlider* volumeSlider;
    QListWidget* playlistWidget;
    
    QPushButton* playButton;
    QPushButton* pauseButton;
    QPushButton* stopButton;
    QPushButton* prevButton;
    QPushButton* nextButton;
    QPushButton* backButton;
    
    QStringList playlist;
    int currentTrackIndex;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onPlay();
    void onPause();
    void onStop();
    void onNext();
    void onPrevious();
    void onVolumeChanged(int value);
    void onProgressChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onPlaylistItemSelected(int index);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onBack();
    void formatTime(qint64 ms, QLabel* label);
};

#endif
