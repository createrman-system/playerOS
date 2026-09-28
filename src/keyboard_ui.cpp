#include "keyboard_ui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

KeyboardUI::KeyboardUI(QWidget* parent)
    : QDialog(parent),
      inputLine(nullptr),
      currentText("")
{
    setWindowTitle("Virtual Keyboard");
    setModal(true);
    setMinimumSize(400, 300);
    
    setupUI();
}

void KeyboardUI::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Input line
    inputLine = new QLineEdit(this);
    mainLayout->addWidget(inputLine);
    
    // Keyboard will be added in createKeyboard()
    createKeyboard();
    
    setLayout(mainLayout);
}

void KeyboardUI::createKeyboard() {
    // Minimal keyboard implementation
    // Framework for future expansion
}

QString KeyboardUI::getText() const {
    if (inputLine) {
        return inputLine->text();
    }
    return currentText;
}

void KeyboardUI::setPrompt(const QString& prompt) {
    // For future use
}

void KeyboardUI::onKeyPressed() {
    // For future use
}

void KeyboardUI::onBackspace() {
    if (inputLine) {
        QString text = inputLine->text();
        if (!text.isEmpty()) {
            text.chop(1);
            inputLine->setText(text);
        }
    }
}

void KeyboardUI::onSpace() {
    if (inputLine) {
        inputLine->setText(inputLine->text() + " ");
    }
}

void KeyboardUI::onSubmit() {
    currentText = inputLine->text();
    accept();
}
