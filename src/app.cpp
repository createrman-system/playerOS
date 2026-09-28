#include "app.h"
#include "home_screen.h"
#include "music_player_app.h"
#include "touch_calibrator_app.h"
#include "settings_app.h"
#include "file_explorer_app.h"
#include "sysinfo_app.h"

#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QTouchDevice>
#include <QDebug>

// ============ AppBase Implementation ============
AppBase::AppBase(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_AcceptTouchEvents);
}

// ============ App Implementation ============
App::App(QWidget* parent) 
    : QMainWindow(parent),
      stackedWidget(nullptr),
      homeScreen(nullptr),
      musicPlayerApp(nullptr),
      touchCalibratorApp(nullptr),
      settingsApp(nullptr),
      fileExplorerApp(nullptr),
      sysInfoApp(nullptr)
{
    setWindowTitle("Music Player OS");
    setFocusPolicy(Qt::StrongFocus);
    
    setupUI();
    createApps();
    setupTouchInput();
    setupFullScreen();
    
    showHomeScreen();
}

App::~App() {
    // Apps are children of stackedWidget, so they'll be deleted automatically
}

void App::setupUI() {
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    
    // Set stylesheet for better touch responsiveness
    setStyleSheet(
        "QMainWindow { background-color: #1e1e1e; }"
        "QPushButton { "
        "    background-color: #0d47a1; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 10px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "} "
        "QPushButton:pressed { background-color: #0a3d91; } "
        "QLabel { color: white; } "
        "QListWidget { background-color: #2a2a2a; color: white; } "
        "QSlider { background-color: #2a2a2a; } "
    );
}

void App::createApps() {
    // Create home screen
    homeScreen = new HomeScreen(stackedWidget);
    stackedWidget->addWidget(homeScreen);
    apps["HomeScreen"] = homeScreen;
    
    // Create music player app
    musicPlayerApp = new MusicPlayerApp(stackedWidget);
    stackedWidget->addWidget(musicPlayerApp);
    apps["MusicPlayer"] = musicPlayerApp;
    
    // Create touch calibrator app
    touchCalibratorApp = new TouchCalibratorApp(stackedWidget);
    stackedWidget->addWidget(touchCalibratorApp);
    apps["TouchCalibrator"] = touchCalibratorApp;
    
    // Create settings app
    settingsApp = new SettingsApp(stackedWidget);
    stackedWidget->addWidget(settingsApp);
    apps["Settings"] = settingsApp;
    
    // Create file explorer app
    fileExplorerApp = new FileExplorerApp(stackedWidget);
    stackedWidget->addWidget(fileExplorerApp);
    apps["FileExplorer"] = fileExplorerApp;
    
    // Create system info app
    sysInfoApp = new SysInfoApp(stackedWidget);
    stackedWidget->addWidget(sysInfoApp);
    apps["SysInfo"] = sysInfoApp;
    
    // Connect app signals
    for (auto app : apps) {
        if (app) {
            connect(app, &AppBase::requestExit, this, &App::onAppExit);
            connect(app, &AppBase::requestHome, this, &App::onShowHome);
            connect(app, &AppBase::launchApp, this, &App::onAppLaunched);
        }
    }
}

void App::setupTouchInput() {
    // Enable touch events
    setAttribute(Qt::WA_AcceptTouchEvents);
    stackedWidget->setAttribute(Qt::WA_AcceptTouchEvents);
    
    // List available touch devices
    const auto& devices = QTouchDevice::devices();
    if (devices.isEmpty()) {
        qWarning() << "No touch devices detected";
    } else {
        for (const auto* device : devices) {
            qDebug() << "Touch device:" << device->name();
        }
    }
}

void App::setupFullScreen() {
    // Set window to fullscreen and maximize
    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();
    
    // Get screen geometry
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        resize(screenGeometry.width(), screenGeometry.height());
        move(0, 0);
        qDebug() << "Screen resolution:" << screenGeometry.width() << "x" << screenGeometry.height();
    }
}

void App::showHomeScreen() {
    if (homeScreen) {
        homeScreen->onAppActivated();
        stackedWidget->setCurrentWidget(homeScreen);
    }
}

void App::onAppLaunched(const QString& appName) {
    if (apps.contains(appName)) {
        AppBase* app = apps[appName];
        app->onAppActivated();
        stackedWidget->setCurrentWidget(app);
        qDebug() << "Launched app:" << appName;
    }
}

void App::onAppExit() {
    AppBase* sender = qobject_cast<AppBase*>(QObject::sender());
    if (sender) {
        sender->onAppDeactivated();
        showHomeScreen();
        qDebug() << "App exited, returning to home screen";
    }
}

void App::onShowHome() {
    AppBase* sender = qobject_cast<AppBase*>(QObject::sender());
    if (sender) {
        sender->onAppDeactivated();
    }
    showHomeScreen();
}
