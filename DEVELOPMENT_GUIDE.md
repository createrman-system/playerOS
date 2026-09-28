# Music Player OS - Development Guide

## Development Environment Setup

### Prerequisites
- Linux development machine (Ubuntu 20.04+ recommended)
- Qt5 development tools
- CMake 3.16+
- Git
- C++17 compatible compiler

### Install Development Tools

```bash
# Ubuntu/Debian
sudo apt-get install -y \
    build-essential \
    cmake \
    qt5-default \
    libqt5gui5-dev \
    libqt5core5a-dev \
    libqt5multimedia5-dev \
    git \
    gdb \
    valgrind

# macOS
brew install cmake qt5
```

### Clone and Setup

```bash
git clone https://github.com/your-username/MusicPlayerOS.git
cd MusicPlayerOS
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Project Architecture

### Class Hierarchy

```
QWidget
  ├── App (QMainWindow)
  │   └── QStackedWidget
  │       ├── AppBase
  │       │   ├── HomeScreen
  │       │   ├── MusicPlayerApp
  │       │   ├── TouchCalibratorApp
  │       │   ├── SettingsApp
  │       │   ├── FileExplorerApp
  │       │   └── SysInfoApp
```

### Signal Flow

```
User Input (Touch/Mouse)
    ↓
Event Handler
    ↓
Slot Function
    ↓
Emit Signal (if needed)
    ↓
Connected Slot in App
    ↓
App State Change
```

## Adding a New App

### 1. Create Header File

Create `include/my_app.h`:

```cpp
#ifndef MY_APP_H
#define MY_APP_H

#include "app.h"
#include <QPushButton>
#include <QLabel>

class MyApp : public AppBase {
    Q_OBJECT

public:
    explicit MyApp(QWidget* parent = nullptr);
    QString getAppName() const override { return "MyApp"; }

private:
    void setupUI();

    QLabel* titleLabel;
    QPushButton* backButton;

private slots:
    void onBack();
};

#endif
```

### 2. Create Source File

Create `src/my_app.cpp`:

```cpp
#include "my_app.h"
#include <QVBoxLayout>
#include <QPainter>

MyApp::MyApp(QWidget* parent)
    : AppBase(parent),
      titleLabel(nullptr),
      backButton(nullptr)
{
    setStyleSheet(
        "MyApp { background-color: #0a0e27; }"
        "QLabel { color: white; }"
        "QPushButton { "
        "    background-color: #1a237e; "
        "    color: white; "
        "    border: 2px solid #3f51b5; "
        "    border-radius: 10px; "
        "    padding: 10px; "
        "} "
    );
    
    setupUI();
}

void MyApp::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);
    
    // Title
    titleLabel = new QLabel("My App", this);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Content
    mainLayout->addStretch();
    
    // Back button
    backButton = new QPushButton("← Back", this);
    connect(backButton, &QPushButton::clicked, this, &MyApp::onBack);
    mainLayout->addWidget(backButton);
    
    setLayout(mainLayout);
}

void MyApp::onBack() {
    emit requestHome();
}

void MyApp::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(10, 14, 39));
    gradient.setColorAt(1, QColor(20, 30, 70));
    painter.fillRect(rect(), gradient);
    QWidget::paintEvent(event);
}
```

### 3. Update CMakeLists.txt

Add to SOURCES and HEADERS sections:

```cmake
set(SOURCES
    # ... existing files ...
    src/my_app.cpp
)

set(HEADERS
    # ... existing files ...
    include/my_app.h
)
```

### 4. Register in App Class

Edit `include/app.h`:

```cpp
class App : public QMainWindow {
    // ... existing code ...

private:
    MyApp* myApp;  // Add this member
};
```

Edit `src/app.cpp` in `createApps()`:

```cpp
void App::createApps() {
    // ... existing apps ...
    
    // Create my app
    myApp = new MyApp(stackedWidget);
    stackedWidget->addWidget(myApp);
    apps["MyApp"] = myApp;
    
    connect(myApp, &AppBase::requestExit, this, &App::onAppExit);
    connect(myApp, &AppBase::requestHome, this, &App::onShowHome);
    connect(myApp, &AppBase::launchApp, this, &App::onAppLaunched);
}
```

### 5. Add to Home Screen

Edit `src/home_screen.cpp` in `createAppButtons()`:

```cpp
QVector<AppInfo> appsList = {
    // ... existing apps ...
    {"MyApp", "🎯 My\nApp", 2, 1},
};
```

### 6. Build and Test

```bash
cd build
cmake ..
make clean
make -j$(nproc)
./MusicPlayerOS
```

## Code Style Guidelines

### Naming Conventions

- Classes: `PascalCase` (e.g., `MusicPlayerApp`)
- Functions: `camelCase` (e.g., `onPlayButtonClicked`)
- Variables: `camelCase` (e.g., `currentTrackIndex`)
- Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_VOLUME`)
- Slots: `onEventName` (e.g., `onButtonClicked`)

### File Organization

```cpp
#ifndef HEADER_GUARD_H
#define HEADER_GUARD_H

#include <Qt includes>
#include "local_includes.h"

// Forward declarations
class ForwardClass;

// Class definition
class MyClass : public BaseClass {
    Q_OBJECT

public:
    explicit MyClass(QWidget* parent = nullptr);
    ~MyClass();

    // Public methods
    void publicMethod();

private:
    // Private methods
    void setupUI();

    // Member variables
    QLabel* label;
    QPushButton* button;

private slots:
    void onButtonClicked();
};

#endif
```

### Documentation

```cpp
// Always document public functions
/// Brief description of what the function does.
/// More detailed explanation if needed.
/// @param param1 Description of param1
/// @return Description of return value
void importantFunction(int param1);

// Comment complex logic
for (int i = 0; i < size; ++i) {
    // Explanation of what this loop does
}
```

## Debugging

### Enable Debug Mode

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

### Run with GDB

```bash
gdb ./MusicPlayerOS
(gdb) run
(gdb) bt  # Backtrace on crash
(gdb) quit
```

### Check for Memory Leaks

```bash
valgrind --leak-check=full ./MusicPlayerOS
```

### Logging

Use Qt's qDebug for logging:

```cpp
#include <QDebug>

qDebug() << "Application started";
qWarning() << "This is a warning";
qCritical() << "Critical error!";
```

View logs on Raspberry Pi:

```bash
journalctl -u MusicPlayerOS -f
```

## Testing

### Unit Testing (Qt TestLib)

Create `tests/test_app.cpp`:

```cpp
#include <QtTest>
#include "app.h"

class TestApp : public QObject {
    Q_OBJECT

private slots:
    void testInitialization();
    void testAppLaunching();
};

void TestApp::testInitialization() {
    QVERIFY(true);
}

QTEST_MAIN(TestApp)
#include "test_app.moc"
```

Add to CMakeLists.txt:

```cmake
enable_testing()
add_executable(test_app tests/test_app.cpp)
add_test(NAME test_app COMMAND test_app)
```

### Manual Testing Checklist

- [ ] App starts without errors
- [ ] All buttons are responsive
- [ ] Touch input works correctly
- [ ] No memory leaks over 1 hour
- [ ] All features function as designed

## Performance Optimization

### Profiling

Use Qt Creator's built-in profiler:
- Tools → Analyzer → QML Profiler

Or use command-line profiling:

```bash
perf record ./MusicPlayerOS
perf report
```

### Memory Optimization

```cpp
// Use smart pointers to prevent leaks
std::unique_ptr<MyClass> obj(new MyClass());

// Defer operations if not needed immediately
QTimer::singleShot(100, [this]() {
    expensiveOperation();
});

// Cache expensive computations
static QString cachedResult = computeExpensiveValue();
```

### CPU Optimization

```cpp
// Batch updates instead of individual redraws
update();  // Schedules single redraw

// Use move semantics for large objects
void setData(QVector<int>&& data) {
    this->data = std::move(data);
}

// Avoid unnecessary copying
const QVector<int>& getData() const {
    return data;
}
```

## Common Pitfalls

### Memory Leaks
```cpp
// DON'T: Parent not set, memory leak
QLabel* label = new QLabel();

// DO: Parent ensures deletion
QLabel* label = new QLabel(this);
```

### Signal/Slot Connection
```cpp
// DON'T: Forget Q_OBJECT macro
class MyClass : public QObject {
signals:
    void mySignal();
};

// DO: Always include Q_OBJECT
class MyClass : public QObject {
    Q_OBJECT
signals:
    void mySignal();
};
```

### Thread Safety
```cpp
// DON'T: Modify GUI from background thread
thread->connect(thread, &QThread::finished, 
                 [this]() { label->setText("Done"); });

// DO: Use Qt::AutoConnection (default) or Qt::QueuedConnection
connect(worker, &Worker::done, this, &App::onDone, Qt::QueuedConnection);
```

## Building for Raspberry Pi

### Cross-Compilation (Optional)

Set up cross-compiler toolchain:

```bash
# Install ARM toolchain
sudo apt-get install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf

# Create CMake toolchain file
cat > toolchain.cmake << EOF
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
EOF

# Build
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake ..
make
```

### Direct Build on Pi

```bash
cd MusicPlayerOS
./build.sh
```

## Git Workflow

```bash
# Create feature branch
git checkout -b feature/my-feature

# Make changes and commit
git add .
git commit -m "Add new feature"

# Push to remote
git push origin feature/my-feature

# Create pull request on GitHub
```

## Release Process

1. Update version in `src/main.cpp`
2. Update `CHANGELOG.md` or release notes
3. Tag release: `git tag v1.0.0`
4. Create GitHub release
5. Build distribution packages

## Resources

- [Qt5 Documentation](https://doc.qt.io/qt-5/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Raspberry Pi Documentation](https://www.raspberrypi.org/documentation/)
- [Qt Signal/Slots Guide](https://doc.qt.io/qt-5/signalsandslots.html)

## Support

For development questions:
1. Check existing code for examples
2. Review Qt documentation
3. Search GitHub issues
4. Ask on Qt forums or Stack Overflow

---

Happy coding! 🚀
