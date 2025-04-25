#pragma once

#include <opencv2/opencv.hpp>
#include <QString>

class Capture {
public:
    static Capture& instance();

    bool initialize();
    bool isReady() const;
    cv::Mat getFrame();

private:
    Capture(); // private constructor for singleton

    QString mode;
    QString path;
    bool loop = false;
    int cameraId = 0;
    int width = 640;
    int height = 480;

    bool ready = false;

    cv::VideoCapture cap;   // used for video/webcam
    cv::Mat image;          // used for static image
};