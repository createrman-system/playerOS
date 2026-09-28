#include "home_screen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QDateTime>
#include <QScreen>
#include <QGuiApplication>
#include <QFont>
#include <QDebug>

HomeScreen::HomeScreen(QWidget* parent)
    : AppBase(parent),
      clockLabel(nullptr),
      dateLabel(nullptr),
      appsGrid(nullptr),
      clockTimer(nullptr)
{
    setStyleSheet(
        "HomeScreen { background-color: #0a0e27; }"
        "QPushButton { "
        "    background-color: #1a237e; "
        "    color: white; "
        "    border: 2px solid #3f51b5; "
        "    border-radius: 12px; "
        "    padding: 15px; "
        "    font-size: 16px; "
        "    font-weight: bold; "
        "    min-width: 100px; "
        "    min-height: 100px; "
        "} "
        "QPushButton:pressed { "
        "    background-color: #0d1a5a; "
        "    border: 2px solid #5c6bc0; "
        "} "
        "QPushButton:hover { "
        "    background-color: #283593; "
        "} "
    );
    
    setupUI();
    
    // Start clock timer
    clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, &HomeScreen::updateTimeDisplay);
    clockTimer->start(1000);  // Update every second
    
    updateTimeDisplay();
}

void HomeScreen::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);
    
    // Top section: Clock and Date
    QHBoxLayout* topLayout = new QHBoxLayout();
    
    clockLabel = new QLabel(this);
    clockLabel->setAlignment(Qt::AlignCenter);
    clockLabel->setFont(QFont("Arial", 48, QFont::Bold));
    clockLabel->setStyleSheet("color: #00bcd4; background-color: transparent;");
    topLayout->addWidget(clockLabel);
    
    dateLabel = new QLabel(this);
    dateLabel->setAlignment(Qt::AlignCenter);
    dateLabel->setFont(QFont("Arial", 20));
    dateLabel->setStyleSheet("color: #80deea; background-color: transparent;");
    topLayout->addStretch();
    topLayout->addWidget(dateLabel);
    topLayout->addStretch();
    
    mainLayout->addLayout(topLayout, 1);
    
    // App buttons grid
    QWidget* gridWidget = new QWidget(this);
    appsGrid = new QGridLayout(gridWidget);
    appsGrid->setSpacing(10);
    appsGrid->setContentsMargins(0, 0, 0, 0);
    
    createAppButtons();
    
    mainLayout->addWidget(gridWidget, 4);
    
    setLayout(mainLayout);
}

void HomeScreen::createAppButtons() {
    if (!appsGrid) return;
    
    // Define apps to display on home screen
    struct AppInfo {
        QString name;
        QString displayName;
        int row;
        int col;
    };
    
    QVector<AppInfo> appsList = {
        {"MusicPlayer", "♫ Music\nPlayer", 0, 0},
        {"FileExplorer", "📁 File\nExplorer", 0, 1},
        {"TouchCalibrator", "✋ Touch\nCalibrate", 1, 0},
        {"Settings", "⚙️ Settings", 1, 1},
        {"SysInfo", "ℹ️ System\nInfo", 2, 0},
    };
    
    for (const auto& app : appsList) {
        QPushButton* button = new QPushButton(app.displayName, this);
        button->setMinimumSize(120, 120);
        button->setMaximumSize(180, 180);
        button->setFont(QFont("Arial", 14, QFont::Bold));
        button->setFocusPolicy(Qt::NoFocus);
        
        // Store app name for later lookup
        buttonToAppMap[button] = app.name;
        
        connect(button, &QPushButton::clicked, this, &HomeScreen::onAppButtonClicked);
        
        appsGrid->addWidget(button, app.row, app.col, Qt::AlignCenter);
    }
    
    // Add stretch to fill remaining space
    appsGrid->setColumnStretch(2, 1);
    appsGrid->setRowStretch(3, 1);
}

void HomeScreen::updateTimeDisplay() {
    QDateTime now = QDateTime::currentDateTime();
    clockLabel->setText(now.toString("hh:mm"));
    dateLabel->setText(now.toString("dddd\nMMMM d, yyyy"));
}

void HomeScreen::onAppButtonClicked() {
    QPushButton* sender = qobject_cast<QPushButton*>(QObject::sender());
    if (sender && buttonToAppMap.contains(sender)) {
        QString appName = buttonToAppMap[sender];
        qDebug() << "Launching app:" << appName;
        emit launchApp(appName);
    }
}

void HomeScreen::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(10, 14, 39));
    gradient.setColorAt(1, QColor(20, 30, 70));
    painter.fillRect(rect(), gradient);
    
    QWidget::paintEvent(event);
}
