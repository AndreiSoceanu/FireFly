#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    static MainWindow& instance();

    void initialize();
    void start();

private slots:
    void updateFrame();

private:
    MainWindow(QWidget *parent = nullptr);

    QLabel* videoLabel;
    QTimer* timer = nullptr;  // Timer is still used for webcam/video modes
};
