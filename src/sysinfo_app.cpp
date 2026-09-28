#include "sysinfo_app.h"
#include "system.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QGroupBox>

SysInfoApp::SysInfoApp(QWidget* parent)
    : AppBase(parent),
      deviceNameLabel(nullptr),
      cpuLabel(nullptr),
      memoryLabel(nullptr),
      diskLabel(nullptr),
      uptimeLabel(nullptr),
      temperatureLabel(nullptr),
      backButton(nullptr),
      updateTimer(nullptr)
{
    setStyleSheet(
        "SysInfoApp { background-color: #0a0e27; }"
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
    );
    
    setupUI();
    
    // Create update timer
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &SysInfoApp::onUpdateInfo);
}

void SysInfoApp::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);
    
    // Title
    QLabel* titleLabel = new QLabel("System Information", this);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Device Info Group
    QGroupBox* deviceGroup = new QGroupBox("Device", this);
    QVBoxLayout* deviceLayout = new QVBoxLayout(deviceGroup);
    
    deviceNameLabel = new QLabel("Device: Raspberry Pi 3", this);
    deviceNameLabel->setFont(QFont("Arial", 13, QFont::Bold));
    deviceLayout->addWidget(deviceNameLabel);
    
    mainLayout->addWidget(deviceGroup);
    
    // System Stats Group
    QGroupBox* statsGroup = new QGroupBox("System Status", this);
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);
    
    cpuLabel = new QLabel("CPU Usage: Loading...", this);
    cpuLabel->setFont(QFont("Courier", 11));
    statsLayout->addWidget(cpuLabel);
    
    memoryLabel = new QLabel("Memory: Loading...", this);
    memoryLabel->setFont(QFont("Courier", 11));
    statsLayout->addWidget(memoryLabel);
    
    diskLabel = new QLabel("Disk Space: Loading...", this);
    diskLabel->setFont(QFont("Courier", 11));
    statsLayout->addWidget(diskLabel);
    
    temperatureLabel = new QLabel("Temperature: Loading...", this);
    temperatureLabel->setFont(QFont("Courier", 11));
    temperatureLabel->setStyleSheet("color: #ffeb3b;");
    statsLayout->addWidget(temperatureLabel);
    
    uptimeLabel = new QLabel("Uptime: Loading...", this);
    uptimeLabel->setFont(QFont("Courier", 11));
    statsLayout->addWidget(uptimeLabel);
    
    mainLayout->addWidget(statsGroup);
    
    // Stretch to push button to bottom
    mainLayout->addStretch();
    
    // Back button
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    
    backButton = new QPushButton("← Back", this);
    backButton->setMaximumWidth(150);
    backButton->setMinimumHeight(45);
    connect(backButton, &QPushButton::clicked, this, &SysInfoApp::onBack);
    bottomLayout->addWidget(backButton);
    
    mainLayout->addLayout(bottomLayout);
    
    setLayout(mainLayout);
}

void SysInfoApp::updateSystemInfo() {
    if (cpuLabel) cpuLabel->setText("CPU: Raspberry Pi 3 ARM (4 cores)");
    if (memoryLabel) memoryLabel->setText("Memory: " + System::getMemoryUsage());
    if (diskLabel) diskLabel->setText("Disk: " + System::getDiskUsage());
    if (temperatureLabel) temperatureLabel->setText("Temperature: " + System::getCPUTemperature());
    if (uptimeLabel) uptimeLabel->setText("Uptime: " + System::getUptime());
}

void SysInfoApp::onUpdateInfo() {
    updateSystemInfo();
}

void SysInfoApp::onBack() {
    emit requestHome();
}

void SysInfoApp::onAppActivated() {
    updateSystemInfo();
    // Update system info every 2 seconds
    if (updateTimer) {
        updateTimer->start(2000);
    }
}

void SysInfoApp::onAppDeactivated() {
    // Stop updating when app is deactivated
    if (updateTimer) {
        updateTimer->stop();
    }
}

void SysInfoApp::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(10, 14, 39));
    gradient.setColorAt(1, QColor(20, 30, 70));
    painter.fillRect(rect(), gradient);
    
    QWidget::paintEvent(event);
}
