#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMap>
#include <memory>

class App;
class HomeScreen;
class MusicPlayerApp;
class TouchCalibratorApp;
class SettingsApp;
class FileExplorerApp;
class SysInfoApp;

// Base class for all applications
class AppBase : public QWidget {
    Q_OBJECT

public:
    explicit AppBase(QWidget* parent = nullptr);
    virtual ~AppBase() = default;
    virtual QString getAppName() const = 0;
    virtual QString getAppIcon() const { return ""; }
    virtual void onAppActivated() {}
    virtual void onAppDeactivated() {}

signals:
    void requestExit();
    void requestHome();
    void launchApp(const QString& appName);
};

// Main application window
class App : public QMainWindow {
    Q_OBJECT

public:
    explicit App(QWidget* parent = nullptr);
    ~App();

protected:
    void setupUI();
    void createApps();
    void showHomeScreen();

private:
    QStackedWidget* stackedWidget;
    QMap<QString, AppBase*> apps;
    
    HomeScreen* homeScreen;
    MusicPlayerApp* musicPlayerApp;
    TouchCalibratorApp* touchCalibratorApp;
    SettingsApp* settingsApp;
    FileExplorerApp* fileExplorerApp;
    SysInfoApp* sysInfoApp;

    void setupTouchInput();
    void setupFullScreen();

private slots:
    void onAppLaunched(const QString& appName);
    void onAppExit();
    void onShowHome();
};

#endif
