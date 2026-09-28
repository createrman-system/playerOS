#include "file_explorer_app.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QStandardPaths>
#include <QDebug>

FileExplorerApp::FileExplorerApp(QWidget* parent)
    : AppBase(parent),
      pathLabel(nullptr),
      fileList(nullptr),
      backNavButton(nullptr),
      parentButton(nullptr),
      homeButton(nullptr),
      currentPath(QDir::homePath())
{
    setStyleSheet(
        "FileExplorerApp { background-color: #0a0e27; }"
        "QLabel { color: white; background-color: transparent; }"
        "QPushButton { "
        "    background-color: #1a237e; "
        "    color: white; "
        "    border: 2px solid #3f51b5; "
        "    border-radius: 8px; "
        "    padding: 8px; "
        "    font-size: 12px; "
        "    font-weight: bold; "
        "} "
        "QPushButton:pressed { "
        "    background-color: #0d1a5a; "
        "    border: 2px solid #5c6bc0; "
        "} "
        "QListWidget { background-color: #1a1a2e; color: white; border: none; } "
        "QListWidget::item:selected { background-color: #3f51b5; } "
    );
    
    setupUI();
    loadDirectory(currentPath);
}

void FileExplorerApp::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    
    // Title
    QLabel* titleLabel = new QLabel("File Explorer", this);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Navigation buttons
    QHBoxLayout* navLayout = new QHBoxLayout();
    
    homeButton = new QPushButton("🏠 Home", this);
    homeButton->setMaximumWidth(120);
    connect(homeButton, &QPushButton::clicked, this, &FileExplorerApp::onHome);
    navLayout->addWidget(homeButton);
    
    parentButton = new QPushButton("⬆ Parent", this);
    parentButton->setMaximumWidth(120);
    connect(parentButton, &QPushButton::clicked, this, &FileExplorerApp::onParentDirectory);
    navLayout->addWidget(parentButton);
    
    navLayout->addStretch();
    
    mainLayout->addLayout(navLayout);
    
    // Current path display
    pathLabel = new QLabel(currentPath, this);
    pathLabel->setFont(QFont("Courier", 10));
    pathLabel->setStyleSheet("color: #80deea; background-color: #1a1a2e; padding: 8px; border-radius: 4px;");
    pathLabel->setWordWrap(true);
    mainLayout->addWidget(pathLabel);
    
    // File list
    fileList = new QListWidget(this);
    fileList->setFont(QFont("Arial", 12));
    connect(fileList, &QListWidget::itemClicked,
            this, &FileExplorerApp::onFileSelected);
    mainLayout->addWidget(fileList);
    
    // Bottom control bar
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    
    backNavButton = new QPushButton("← Back to Home", this);
    connect(backNavButton, &QPushButton::clicked, this, &FileExplorerApp::onBack);
    bottomLayout->addWidget(backNavButton);
    
    mainLayout->addLayout(bottomLayout);
    
    setLayout(mainLayout);
}

void FileExplorerApp::loadDirectory(const QString& path) {
    currentDir = QDir(path);
    currentPath = path;
    
    // Update path label
    if (pathLabel) {
        pathLabel->setText("📁 " + path);
    }
    
    // Clear and load file list
    if (fileList) {
        fileList->clear();
        
        QFileInfoList entries = currentDir.entryInfoList(
            QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
            QDir::DirsFirst | QDir::Name
        );
        
        for (const auto& entry : entries) {
            QString displayName = entry.fileName();
            
            if (entry.isDir()) {
                displayName = "📁 " + displayName;
            } else {
                // Show file size
                qint64 size = entry.size();
                QString sizeStr;
                if (size < 1024) {
                    sizeStr = QString::number(size) + "B";
                } else if (size < 1024 * 1024) {
                    sizeStr = QString::number(size / 1024) + "KB";
                } else {
                    sizeStr = QString::number(size / (1024 * 1024)) + "MB";
                }
                displayName = "📄 " + displayName + " (" + sizeStr + ")";
            }
            
            fileList->addItem(displayName);
        }
        
        if (entries.isEmpty()) {
            fileList->addItem("(empty directory)");
        }
        
        qDebug() << "Loaded directory:" << path << "with" << entries.size() << "items";
    }
}

void FileExplorerApp::onFileSelected(QListWidgetItem* item) {
    if (!item) return;
    
    // Get the actual filename (remove emoji prefix)
    QString displayName = item->text();
    QString actualName = displayName;
    
    if (displayName.startsWith("📁 ")) {
        actualName = displayName.mid(3);
    } else if (displayName.startsWith("📄 ")) {
        // Extract filename before the size info
        actualName = displayName.mid(3).split(" (")[0];
    }
    
    if (actualName == "(empty directory)") {
        return;
    }
    
    QString fullPath = currentDir.absoluteFilePath(actualName);
    QFileInfo fileInfo(fullPath);
    
    if (fileInfo.isDir()) {
        // Navigate into directory
        loadDirectory(fullPath);
    } else {
        // Show file info
        qDebug() << "Selected file:" << fullPath;
    }
}

void FileExplorerApp::onParentDirectory() {
    if (currentDir.cdUp()) {
        loadDirectory(currentDir.absolutePath());
    }
}

void FileExplorerApp::onHome() {
    loadDirectory(QDir::homePath());
}

void FileExplorerApp::onBack() {
    emit requestHome();
}

void FileExplorerApp::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    
    // Draw gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(10, 14, 39));
    gradient.setColorAt(1, QColor(20, 30, 70));
    painter.fillRect(rect(), gradient);
    
    QWidget::paintEvent(event);
}
