#ifndef TOUCH_CALIBRATOR_APP_H
#define TOUCH_CALIBRATOR_APP_H

#include "app.h"
#include <QLabel>
#include <QPushButton>
#include <QPoint>
#include <QVector>
#include <QEvent>

struct TouchPoint {
    QPoint screenPos;
    QPoint touchPos;
};

class TouchCalibratorApp : public AppBase {
    Q_OBJECT

public:
    explicit TouchCalibratorApp(QWidget* parent = nullptr);
    QString getAppName() const override { return "TouchCalibrator"; }
    void onAppActivated() override;
    void onAppDeactivated() override;

private:
    void setupUI();
    void startCalibration();
    void drawCalibrationPoint();
    void saveCalibration();
    void loadCalibration();

    QLabel* instructionLabel;
    QLabel* calibrationDisplay;
    QPushButton* startButton;
    QPushButton* cancelButton;
    QPushButton* saveButton;
    
    QVector<TouchPoint> calibrationPoints;
    int currentPointIndex;
    bool isCalibrating;
    
    // Calibration parameters
    struct CalibrationData {
        float xScale;
        float yScale;
        float xOffset;
        float yOffset;
    } calibrationData;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    bool event(QEvent* event) override;

private slots:
    void onStartCalibration();
    void onCancelCalibration();
    void onSaveCalibration();
    void onBack();
};

#endif
