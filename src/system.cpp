#include "system.h"
#include <QProcess>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

QString System::getSystemTime() {
    return QDateTime::currentDateTime().toString("hh:mm:ss");
}

QString System::getSystemDate() {
    return QDateTime::currentDateTime().toString("dddd, MMMM d, yyyy");
}

QString System::getCPUTemperature() {
    // Read CPU temperature from Raspberry Pi thermal zone
    QFile thermalFile("/sys/class/thermal/thermal_zone0/temp");
    
    if (thermalFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString temp = thermalFile.readAll().trimmed();
        thermalFile.close();
        
        bool ok;
        int tempInt = temp.toInt(&ok);
        if (ok) {
            // Temperature is in millidegrees, convert to Celsius
            float celsius = tempInt / 1000.0f;
            return QString::number(celsius, 'f', 1) + "°C";
        }
    }
    
    return "N/A";
}

QString System::getMemoryUsage() {
    QProcess process;
    process.start("free", QStringList() << "-h");
    process.waitForFinished();
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');
    
    if (lines.size() >= 2) {
        QStringList parts = lines[1].split(QRegExp("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            return parts[1] + " / " + parts[0];  // Used / Total
        }
    }
    
    return "N/A";
}

QString System::getDiskUsage() {
    QProcess process;
    process.start("df", QStringList() << "-h" << "/");
    process.waitForFinished();
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');
    
    if (lines.size() >= 2) {
        QStringList parts = lines[1].split(QRegExp("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            return parts[2] + " / " + parts[1];  // Used / Total
        }
    }
    
    return "N/A";
}

QString System::getDeviceName() {
    QProcess process;
    process.start("hostname", QStringList());
    process.waitForFinished();
    
    QString hostname = process.readAllStandardOutput().trimmed();
    if (!hostname.isEmpty()) {
        return hostname;
    }
    
    return "Raspberry Pi";
}

QString System::getUptime() {
    QProcess process;
    process.start("uptime", QStringList() << "-p");
    process.waitForFinished();
    
    QString output = process.readAllStandardOutput().trimmed();
    if (!output.isEmpty()) {
        return output;
    }
    
    return "N/A";
}

bool System::loadTouchCalibration() {
    return QFile::exists(getTouchCalibrationPath());
}

bool System::saveTouchCalibration(const QString& calibrationData) {
    QString path = getTouchCalibrationPath();
    QFile file(path);
    
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(calibrationData.toUtf8());
        file.close();
        return true;
    }
    
    return false;
}

QString System::getTouchCalibrationPath() {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    
    // Create config directory if it doesn't exist
    QDir dir(configDir);
    if (!dir.exists("MusicPlayerOS")) {
        dir.mkdir("MusicPlayerOS");
    }
    
    return configDir + "/MusicPlayerOS/touch_calibration.json";
}
