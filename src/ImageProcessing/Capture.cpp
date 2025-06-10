#include "Capture.h"
#include "AppConfig.h"
#include <QDebug>
#include <opencv2/opencv.hpp>

Capture::Capture() {
    mode = AppConfig::instance().get_mode();
    path = AppConfig::instance().get_path();
    loop = AppConfig::instance().get_loop();
    cameraId = AppConfig::instance().get_cameraId();
    width = AppConfig::instance().get_width();
    height = AppConfig::instance().get_height();
}

Capture& Capture::instance() {
    static Capture instance;
    return instance;
}

bool Capture::initialize() {
    if (mode == "image") {
        image = cv::imread(path.toStdString());
        if (image.empty()) {
            qWarning() << "❌ Could not load image from path:" << path;
            ready = false;
        } else {
            cv::resize(image, image, cv::Size(width, height));
            ready = true;
            qDebug() << "✅ Loaded image:" << path;
        }

    } else if (mode == "video") {
        cap.open(path.toStdString());
        if (!cap.isOpened()) {
            qWarning() << "❌ Failed to open video file:" << path;
            ready = false;
        } else {
            // Read a dummy frame to enforce resolution
            cv::Mat dummy;
            cap.read(dummy);
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);  // rewind to start

            if (!dummy.empty()) {
                cv::resize(dummy, dummy, cv::Size(width, height));
                qDebug() << "✅ Video resolution set to" << width << "x" << height;
            }
            ready = true;
            qDebug() << "✅ Opened video:" << path;
        }

    } else if (mode == "webcam") {
        cap.open(cameraId, cv::CAP_V4L2);  // Use V4L2 for Linux
        if (cap.isOpened()) {
            cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
            cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
        }

        if (!cap.isOpened()) {
            qWarning() << "❌ Failed to open webcam!!!";
            ready = false;
        } else {
            ready = true;
            qDebug() << "✅ Opened webcam with ID:" << cameraId;
        }

    } else {
        qWarning() << "❌ Unknown mode:" << mode;
        ready = false;
    }

    return ready;
}

bool Capture::isReady() const {
    return ready;
}

cv::Mat Capture::getFrame() {
    if (!ready) return {};

    if (mode == "image") {
        return image.clone(); 
    }

    if (mode == "video" || mode == "webcam") {
        cv::Mat frame;
        cap >> frame;

        // for looping videos
        if (mode == "video" && frame.empty() && loop) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            cap >> frame;
        }

        return frame;
    }

    return {};
}
