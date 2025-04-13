#include "Capture.h"
#include "AppConfig.h"

Capture::Capture() {
    // config values
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
        ready = !image.empty();
    } else if (mode == "video") {
        cap.open(path.toStdString());
        ready = cap.isOpened();
    } else if (mode == "webcam") {
        cap.open(cameraId);
        ready = cap.isOpened();
    } else {
        ready = false;
    }

    return ready;
}

bool Capture::isReady() const {
    return ready;
}

cv::Mat Capture::getFrame() {
    if (mode == "image") {
        return image.clone(); // clone so it's safe to modify
    }

    if (mode == "video" || mode == "webcam") {
        cv::Mat frame;
        cap >> frame;

        if (mode == "video" && frame.empty() && loop) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            cap >> frame;
        }

        return frame;
    }

    return {};
}
