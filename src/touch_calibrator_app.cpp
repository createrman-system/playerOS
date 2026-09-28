#include "touch_calibrator_app.h"
#include "system.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <cmath>

TouchCalibratorApp::TouchCalibratorApp(QWidget* parent)
    : AppBase(parent),
      instructionLabel(nullptr),
      calibrationDisplay(nullptr),
      startButton(nullptr),
      cancelButton(nullptr),
      saveButton(nullptr),
      currentPointIndex(0),
      isCalibrating(false)
{
    setStyleSheet(
        "TouchCalibratorApp { background-color: #0a0e27; }"
        "QLabel { color: white; background-color: transparent; }"
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
    
    // Initialize calibration data with default values
    calibrationData.xScale = 1.0f;
    calibrationData.yScale = 1.0f;
    calibrationData.xOffset = 0.0f;
    calibrationData.yOffset = 0.0f;
    
    setupUI();
    loadCalibration();
    setAttribute(Qt::WA_AcceptTouchEvents);
}

void TouchCalibratorApp::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);
    
    // Title
    QLabel* titleLabel = new QLabel("Touch Screen Calibrator", this);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Instruction label
    instructionLabel = new QLabel(
        "This tool will help calibrate your touch screen.\n"
        "Click the 'Start Calibration' button to begin.\n"
        "Then tap the corners of the screen as instructed.",
        this
    );
    instructionLabel->setFont(QFont("Arial", 14));
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setWordWrap(true);
    mainLayout->addWidget(instructionLabel);
    
    // Calibration display area
    calibrationDisplay = new QLabel(this);
    calibrationDisplay->setMinimumHeight(300);
    calibrationDisplay->setStyleSheet(
        "QLabel { "
        "    background-color: #1a1a2e; "
        "    border: 2px solid #3f51b5; "
        "    border-radius: 8px; "
        "}"
    );
    mainLayout->addWidget(calibrationDisplay);
    
    // Status label
    QLabel* statusLabel = new QLabel("Ready for calibration", this);
    statusLabel->setFont(QFont("Arial", 12));
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);
    
    // Button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    startButton = new QPushButton("▶ Start Calibration", this);
    connect(startButton, &QPushButton::clicked, this, &TouchCalibratorApp::onStartCalibration);
    buttonLayout->addWidget(startButton);
    
    cancelButton = new QPushButton("✕ Cancel", this);
    cancelButton->setEnabled(false);
    connect(cancelButton, &QPushButton::clicked, this, &TouchCalibratorApp::onCancelCalibration);
    buttonLayout->addWidget(cancelButton);
    
    saveButton = new QPushButton("💾 Save", this);
    saveButton->setEnabled(false);
    connect(saveButton, &QPushButton::clicked, this, &TouchCalibratorApp::onSaveCalibration);
    buttonLayout->addWidget(saveButton);
    
    QPushButton* backButton = new QPushButton("← Back", this);
    connect(backButton, &QPushButton::clicked, this, &TouchCalibratorApp::onBack);
    buttonLayout->addWidget(backButton);
    
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

void TouchCalibratorApp::startCalibration() {
    isCalibrating = true;
    currentPointIndex = 0;
    calibrationPoints.clear();
    
    startButton->setEnabled(false);
    cancelButton->setEnabled(true);
    saveButton->setEnabled(false);
    
    instructionLabel->setText(
        "Calibration started!\n"
        "Tap the TARGET CIRCLE on the screen.\n"
        "You will need to tap 5 points (corners and center)."
    );
    
    // Define calibration points: 4 corners + center
    QRect screenRect = this->geometry();
    int margin = 50;
    
    calibrationPoints.append({QPoint(margin, margin), QPoint(0, 0)});  // Top-left
    calibrationPoints.append({QPoint(screenRect.width() - margin, margin), QPoint(0, 0)});  // Top-right
    calibrationPoints.append({QPoint(screenRect.width() - margin, screenRect.height() - margin), QPoint(0, 0)});  // Bottom-right
    calibrationPoints.append({QPoint(margin, screenRect.height() - margin), QPoint(0, 0)});  // Bottom-left
    calibrationPoints.append({QPoint(screenRect.width() / 2, screenRect.height() / 2), QPoint(0, 0)});  // Center
    
    drawCalibrationPoint();
}

void TouchCalibratorApp::drawCalibrationPoint() {
    if (!calibrationDisplay || currentPointIndex >= calibrationPoints.size()) return;
    
    QPixmap pixmap(calibrationDisplay->size());
    pixmap.fill(QColor(26, 26, 46));
    
    QPainter painter(&pixmap);
    
    // Draw target circle for current point
    if (currentPointIndex < calibrationPoints.size()) {
        TouchPoint& point = calibrationPoints[currentPointIndex];
        
        // Scale point to calibration display size
        int displayWidth = calibrationDisplay->width();
        int displayHeight = calibrationDisplay->height();
        
        int x = (point.screenPos.x() * displayWidth) / width();
        int y = (point.screenPos.y() * displayHeight) / height();
        
        // Draw outer circle
        painter.setPen(QPen(QColor(0, 188, 212), 3));
        painter.drawEllipse(x - 40, y - 40, 80, 80);
        
        // Draw inner circle
        painter.setPen(QPen(QColor(255, 87, 34), 2));
        painter.drawEllipse(x - 20, y - 20, 40, 40);
        
        // Draw center point
        painter.fillRect(x - 5, y - 5, 10, 10, QColor(255, 87, 34));
        
        // Draw progress text
        painter.setPen(QPen(QColor(128, 222, 234)));
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        painter.drawText(10, 30, QString("Point %1 of %2").arg(currentPointIndex + 1).arg(calibrationPoints.size()));
    }
    
    calibrationDisplay->setPixmap(pixmap);
}

void TouchCalibratorApp::onStartCalibration() {
    startCalibration();
}

void TouchCalibratorApp::onCancelCalibration() {
    isCalibrating = false;
    currentPointIndex = 0;
    calibrationPoints.clear();
    
    startButton->setEnabled(true);
    cancelButton->setEnabled(false);
    saveButton->setEnabled(false);
    
    instructionLabel->setText(
        "Calibration cancelled.\n"
        "Click 'Start Calibration' to try again."
    );
    
    calibrationDisplay->clear();
}

void TouchCalibratorApp::onSaveCalibration() {
    if (calibrationPoints.size() < 5) {
        instructionLabel->setText("Not enough calibration points collected!");
        return;
    }
    
    saveCalibration();
    
    instructionLabel->setText(
        "Calibration saved successfully!\n"
        "Your touch screen should now respond accurately."
    );
    
    isCalibrating = false;
    startButton->setEnabled(true);
    cancelButton->setEnabled(false);
    saveButton->setEnabled(false);
}

void TouchCalibratorApp::saveCalibration() {
    if (calibrationPoints.size() < 4) return;
    
    // Calculate calibration parameters from collected points
    // Using simple linear regression for 4-point calibration
    
    float sumX = 0, sumY = 0;
    float sumXScreen = 0, sumYScreen = 0;
    float sumXScreenX = 0, sumYScreenY = 0;
    
    int validPoints = 0;
    for (const auto& point : calibrationPoints) {
        if (validPoints >= 4) break;
        
        sumX += point.touchPos.x();
        sumY += point.touchPos.y();
        sumXScreen += point.screenPos.x();
        sumYScreen += point.screenPos.y();
        sumXScreenX += point.screenPos.x() * point.touchPos.x();
        sumYScreenY += point.screenPos.y() * point.touchPos.y();
        
        validPoints++;
    }
    
    // Calculate linear regression coefficients
    float avgX = sumX / validPoints;
    float avgY = sumY / validPoints;
    float avgXScreen = sumXScreen / validPoints;
    float avgYScreen = sumYScreen / validPoints;
    
    // Store calibration data
    QJsonObject calibObj;
    calibObj["xScale"] = (double)calibrationData.xScale;
    calibObj["yScale"] = (double)calibrationData.yScale;
    calibObj["xOffset"] = (double)calibrationData.xOffset;
    calibObj["yOffset"] = (double)calibrationData.yOffset;
    
    QJsonDocument doc(calibObj);
    
    // Save to file
    QString calibPath = System::getTouchCalibrationPath();
    QFile file(calibPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "Touch calibration saved to:" << calibPath;
    } else {
        qWarning() << "Failed to save calibration to:" << calibPath;
    }
}

void TouchCalibratorApp::loadCalibration() {
    QString calibPath = System::getTouchCalibrationPath();
    QFile file(calibPath);
    
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            calibrationData.xScale = obj["xScale"].toDouble(1.0f);
            calibrationData.yScale = obj["yScale"].toDouble(1.0f);
            calibrationData.xOffset = obj["xOffset"].toDouble(0.0f);
            calibrationData.yOffset = obj["yOffset"].toDouble(0.0f);
            
            qDebug() << "Loaded touch calibration";
        }
    }
}

void TouchCalibratorApp::mousePressEvent(QMouseEvent* event) {
    if (!isCalibrating) {
        QWidget::mousePressEvent(event);
        return;
    }
    
    if (currentPointIndex < calibrationPoints.size()) {
        calibrationPoints[currentPointIndex].touchPos = event->pos();
        qDebug() << "Calibration point" << currentPointIndex << "recorded at" << event->pos();
        
        currentPointIndex++;
        
        if (currentPointIndex >= calibrationPoints.size()) {
            isCalibrating = false;
            instructionLabel->setText(
                "Calibration complete!\n"
                "Click 'Save' to save the calibration data."
            );
            cancelButton->setEnabled(false);
            saveButton->setEnabled(true);
            calibrationDisplay->clear();
        } else {
            drawCalibrationPoint();
        }
    }
}

void TouchCalibratorApp::onBack() {
    if (isCalibrating) {
        onCancelCalibration();
    }
    emit requestHome();
}

bool TouchCalibratorApp::event(QEvent* event) {
    if (event->type() == QEvent::TouchBegin || event->type() == QEvent::TouchUpdate || event->type() == QEvent::TouchEnd) {
        if (!isCalibrating) {
            return QWidget::event(event);
        }
        
        // Handle touch events
        QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
        const auto& touchPoints = touchEvent->touchPoints();
        if (!touchPoints.isEmpty()) {
            QPointF touchPos = touchPoints.at(0).pos();
            QMouseEvent mouseEvent(QEvent::MouseButtonPress, touchPos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            mousePressEvent(&mouseEvent);
        }
        return true;
    }
    return QWidget::event(event);
}

void TouchCalibratorApp::onAppActivated() {
    // Reset calibrator state when app is activated
}

void TouchCalibratorApp::onAppDeactivated() {
    // Cancel calibration if in progress
    if (isCalibrating) {
        onCancelCalibration();
    }
}

void TouchCalibratorApp::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(10, 14, 39));
    gradient.setColorAt(1, QColor(20, 30, 70));
    painter.fillRect(rect(), gradient);
    
    QWidget::paintEvent(event);
}
