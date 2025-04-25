#include "Capture.h"
#include "AppConfig.h"
#include <QDebug>
#include <opencv2/opencv.hpp>

Capture::Capture() {
    mode = AppConfig::instance().value("mode/type").toString();
    path = AppConfig::instance().value("mode/path").toString();
    loop = AppConfig::instance().value("video/loop", true).toBool();
    cameraId = AppConfig::instance().value("camera/id", 0).toInt();
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
            int w = AppConfig::instance().value("display/width", 640).toInt();
            int h = AppConfig::instance().value("display/height", 480).toInt();
            cv::resize(image, image, cv::Size(w, h));

            ready = true;
            qDebug() << "✅ Loaded image:" << path;
        }

    } else if (mode == "video") {
        cap.open(path.toStdString());
        if (!cap.isOpened()) {
            qWarning() << "❌ Failed to open video file:" << path;
            ready = false;
        } else {
            int w = AppConfig::instance().value("display/width", 640).toInt();
            int h = AppConfig::instance().value("display/height", 480).toInt();

            // Read a dummy frame to enforce resolution
            cv::Mat dummy;
            cap.read(dummy);
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);  // rewind to start

            if (!dummy.empty()) {
                cv::resize(dummy, dummy, cv::Size(w, h));
                qDebug() << "✅ Video resolution set to" << w << "x" << h;
            }
            ready = true;
            qDebug() << "✅ Opened video:" << path;
        }

    } else if (mode == "webcam") {
        cap.open(cameraId, cv::CAP_V4L2);  // Use V4L2 for Linux
        if (cap.isOpened()) {
            int w = AppConfig::instance().value("display/width", 640).toInt();
            int h = AppConfig::instance().value("display/height", 480).toInt();
            cap.set(cv::CAP_PROP_FRAME_WIDTH, w);
            cap.set(cv::CAP_PROP_FRAME_HEIGHT, h);
        }

        if (!cap.isOpened()) {
            qWarning() << "❌ Failed to open webcam";
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
        return image.clone();  // always return a copy
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
