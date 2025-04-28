#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QCheckBox> 

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    static MainWindow& instance();

    void initialize();
    void start();

private slots:
    void updateFrame();
    void toggleCLAHE(int state);
    void toggleBilateral(int state);
    void toggleYOLO(int state);

private:
    MainWindow(QWidget *parent = nullptr);

    QLabel* videoLabel;
    QTimer* timer = nullptr;  // Timer is still used for webcam/video modes

    QCheckBox* claheCheckBox;
    QCheckBox* bilateralCheckBox;
    QCheckBox* yoloCheckBox;
};
