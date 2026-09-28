#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "app.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

class HomeScreen : public AppBase {
    Q_OBJECT

public:
    explicit HomeScreen(QWidget* parent = nullptr);
    QString getAppName() const override { return "HomeScreen"; }

private:
    void setupUI();
    void createAppButtons();
    void updateClock();

    QLabel* clockLabel;
    QLabel* dateLabel;
    QGridLayout* appsGrid;
    QTimer* clockTimer;
    QMap<QPushButton*, QString> buttonToAppMap;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onAppButtonClicked();
    void updateTimeDisplay();
};

#endif
