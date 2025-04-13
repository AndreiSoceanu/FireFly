#pragma once

#include <opencv2/opencv.hpp>
#include <QString>

class Capture {
public:
    static Capture& instance();

    bool initialize();             // Initializes based on config
    bool isReady() const;         // Whether media is opened/loaded
    cv::Mat getFrame();           // Returns current or next frame

private:
    Capture();                    // Singleton - private constructor

    QString mode;
    QString path;
    bool loop = false;
    int cameraId = 0;

    cv::VideoCapture cap;
    cv::Mat image;
    bool ready = false;
};
