#ifndef SYSTEM_H
#define SYSTEM_H

#include <QString>
#include <QDateTime>
#include <QSystemInfo>

class System {
public:
    static QString getSystemTime();
    static QString getSystemDate();
    static QString getCPUTemperature();
    static QString getMemoryUsage();
    static QString getDiskUsage();
    static QString getDeviceName();
    static QString getUptime();
    
    // Touch calibration
    static bool loadTouchCalibration();
    static bool saveTouchCalibration(const QString& calibrationData);
    static QString getTouchCalibrationPath();
};

#endif
