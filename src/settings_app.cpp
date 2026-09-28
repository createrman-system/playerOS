#include "settings_app.h"
#include "system.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QGroupBox>

SettingsApp::SettingsApp(QWidget* parent)
    : AppBase(parent),
      volumeLabel(nullptr),
      volumeSlider(nullptr),
      calibrateButton(nullptr),
      backButton(nullptr),
      systemInfoLabel(nullptr)
{
    setStyleSheet(
        "SettingsApp { background-color: #0a0e27; }"
        "QLabel { color: white; background-color: transparent; }"
        "QGroupBox { color: white; border: 2px solid #3f51b5; border-radius: 8px; padding-top: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px; }"
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
    );
    
    setupUI();
}

void SettingsApp::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);
    
    // Title
    QLabel* titleLabel = new QLabel("Settings", this);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Volume Control Group
    QGroupBox* volumeGroup = new QGroupBox("Audio Settings", this);
    QVBoxLayout* volumeLayout = new QVBoxLayout(volumeGroup);
    
    volumeLabel = new QLabel("Master Volume: 70%", this);
    volumeLabel->setFont(QFont("Arial", 14));
    volumeLayout->addWidget(volumeLabel);
    
    QHBoxLayout* sliderLayout = new QHBoxLayout();
    QLabel* muteLabel = new QLabel("🔇", this);
    sliderLayout->addWidget(muteLabel);
    
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setMinimum(0);
    volumeSlider->setMaximum(100);
    volumeSlider->setValue(70);
    connect(volumeSlider, QOverload<int>::of(&QSlider::valueChanged),
            this, &SettingsApp::onVolumeChanged);
    sliderLayout->addWidget(volumeSlider);
    
    QLabel* loudLabel = new QLabel("🔊", this);
    sliderLayout->addWidget(loudLabel);
    
    volumeLayout->addLayout(sliderLayout);
    mainLayout->addWidget(volumeGroup);
    
    // Touch Calibration Group
    QGroupBox* touchGroup = new QGroupBox("Touch Screen", this);
    QVBoxLayout* touchLayout = new QVBoxLayout(touchGroup);
    
    QLabel* touchInfoLabel = new QLabel(
        "If the touch screen is not responding accurately,\n"
        "you can recalibrate it here.",
        this
    );
    touchInfoLabel->setFont(QFont("Arial", 12));
    touchInfoLabel->setWordWrap(true);
    touchLayout->addWidget(touchInfoLabel);
    
    calibrateButton = new QPushButton("✋ Calibrate Touch Screen", this);
    calibrateButton->setMinimumHeight(50);
    connect(calibrateButton, &QPushButton::clicked, this, &SettingsApp::onCalibrate);
    touchLayout->addWidget(calibrateButton);
    
    mainLayout->addWidget(touchGroup);
    
    // System Info Group
    QGroupBox* sysGroup = new QGroupBox("System Information", this);
    QVBoxLayout* sysLayout = new QVBoxLayout(sysGroup);
    
    systemInfoLabel = new QLabel(this);
    systemInfoLabel->setFont(QFont("Courier", 10));
    systemInfoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    updateSystemInfo();
    sysLayout->addWidget(systemInfoLabel);
    
    mainLayout->addWidget(sysGroup);
    
    // Stretch to push buttons to bottom
    mainLayout->addStretch();
    
    // Back button
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    
    backButton = new QPushButton("← Back", this);
    backButton->setMaximumWidth(150);
    backButton->setMinimumHeight(45);
    connect(backButton, &QPushButton::clicked, this, &SettingsApp::onBack);
    bottomLayout->addWidget(backButton);
    
    mainLayout->addLayout(bottomLayout);
    
    setLayout(mainLayout);
}

void SettingsApp::onVolumeChanged(int value) {
    volumeLabel->setText(QString("Master Volume: %1%").arg(value));
    qDebug() << "Volume changed to:" << value;
}

void SettingsApp::onCalibrate() {
    qDebug() << "Launching touch calibrator";
    emit launchApp("TouchCalibrator");
}

void SettingsApp::updateSystemInfo() {
    if (!systemInfoLabel) return;
    
    QString info;
    info += "Device: Raspberry Pi 3\n";
    info += "OS: Raspberry Pi OS (Linux)\n";
    info += "App: Music Player OS v1.0\n";
    info += "\n";
    info += "Temperature: " + System::getCPUTemperature() + "\n";
    info += "Memory: " + System::getMemoryUsage() + "\n";
    info += "Disk: " + System::getDiskUsage() + "\n";
    info += "Uptime: " + System::getUptime() + "\n";
    
    systemInfoLabel->setText(info);
}

void SettingsApp::onBack() {
    emit requestHome();
}

void SettingsApp::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(10, 14, 39));
    gradient.setColorAt(1, QColor(20, 30, 70));
    painter.fillRect(rect(), gradient);
    
    QWidget::paintEvent(event);
}
