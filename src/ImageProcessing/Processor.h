#pragma once

#include <opencv2/opencv.hpp>

class Processor {
public:
    static Processor& instance();

    void initialize();             // Setup any models, parameters
    cv::Mat process(const cv::Mat& input);  // Apply processing and return output

private:
    Processor();                   // Singleton pattern

    float confidenceThreshold = 0.5f;

    // You can later add model objects (e.g., cv::dnn::Net)
};
