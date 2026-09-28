#ifndef FILE_EXPLORER_APP_H
#define FILE_EXPLORER_APP_H

#include "app.h"
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QDir>

class FileExplorerApp : public AppBase {
    Q_OBJECT

public:
    explicit FileExplorerApp(QWidget* parent = nullptr);
    QString getAppName() const override { return "FileExplorer"; }

private:
    void setupUI();
    void loadDirectory(const QString& path);

    QLabel* pathLabel;
    QListWidget* fileList;
    QPushButton* backNavButton;
    QPushButton* parentButton;
    QPushButton* homeButton;
    
    QDir currentDir;
    QString currentPath;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onFileSelected(QListWidgetItem* item);
    void onParentDirectory();
    void onHome();
    void onBack();
};

#endif
