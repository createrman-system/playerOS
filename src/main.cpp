#include <QApplication>
#include <QScreen>
#include <QDebug>
#include "app.h"

int main(int argc, char* argv[]) {
    // Create Qt application
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Music Player OS");
    app.setApplicationVersion("1.0.0");
    app.setApplicationDisplayName("Music Player OS");
    
    // Configure for touch screen
    qputenv("QT_QPA_PLATFORM", "linuxfb");
    qputenv("QT_QPA_GENERIC_PLUGINS", "tslib:/dev/input/touchscreen0");
    
    // Create and show main window
    App mainWindow;
    
    // Log display info
    QScreen* screen = QApplication::primaryScreen();
    if (screen) {
        qDebug() << "Display:" << screen->geometry().width() << "x" << screen->geometry().height();
        qDebug() << "DPI:" << screen->logicalDotsPerInch();
    }
    
    qDebug() << "Music Player OS started successfully";
    
    return app.exec();
}
