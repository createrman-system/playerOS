#ifndef SETTINGS_APP_H
#define SETTINGS_APP_H

#include "app.h"
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

class SettingsApp : public AppBase {
    Q_OBJECT

public:
    explicit SettingsApp(QWidget* parent = nullptr);
    QString getAppName() const override { return "Settings"; }

private:
    void setupUI();

    // Settings UI components
    QLabel* volumeLabel;
    QSlider* volumeSlider;
    QPushButton* calibrateButton;
    QPushButton* backButton;
    QLabel* systemInfoLabel;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onVolumeChanged(int value);
    void onCalibrate();
    void onBack();
    void updateSystemInfo();
};

#endif
