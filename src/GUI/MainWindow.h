#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    static MainWindow& instance();
    void initialize();             // Call after Capture/Processor initialized
    void start();                  // Starts the processing loop

private slots:
    void updateFrame();

private:
    MainWindow(QWidget *parent = nullptr);
    QLabel* videoLabel;
    QTimer* timer;
};
