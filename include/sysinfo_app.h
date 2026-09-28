#ifndef SYSINFO_APP_H
#define SYSINFO_APP_H

#include "app.h"
#include <QLabel>
#include <QPushButton>
#include <QTimer>

class SysInfoApp : public AppBase {
    Q_OBJECT

public:
    explicit SysInfoApp(QWidget* parent = nullptr);
    QString getAppName() const override { return "SysInfo"; }
    void onAppActivated() override;
    void onAppDeactivated() override;

private:
    void setupUI();
    void updateSystemInfo();

    QLabel* deviceNameLabel;
    QLabel* cpuLabel;
    QLabel* memoryLabel;
    QLabel* diskLabel;
    QLabel* uptimeLabel;
    QLabel* temperatureLabel;
    QPushButton* backButton;
    QTimer* updateTimer;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onUpdateInfo();
    void onBack();
};

#endif
