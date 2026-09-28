#ifndef KEYBOARD_UI_H
#define KEYBOARD_UI_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class KeyboardUI : public QDialog {
    Q_OBJECT

public:
    KeyboardUI(QWidget* parent = nullptr);
    QString getText() const;
    void setPrompt(const QString& prompt);

private:
    void setupUI();
    void createKeyboard();

    QLineEdit* inputLine;
    QString currentText;
    QPushButton* keys[26];
    
private slots:
    void onKeyPressed();
    void onBackspace();
    void onSpace();
    void onSubmit();
};

#endif
