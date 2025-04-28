#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <fstream>

class Processor {
public:
    static Processor& instance();

    void initialize();             // Setup any models, parameters
    cv::Mat process(const cv::Mat& input);  // Apply processing and return output

    void setApplyClahe(bool on);
    void setApplyBilateral(bool on);
    void setApplyYolo(bool on);

private:
    Processor();                   // Singleton pattern

    void detectHumans(cv::Mat& output);

    float confidenceThreshold = 0.5f;
    
    float claheClipLimit = 2.0f;
    int claheTileGridSize = 8;

    int bilateralFilterDiameter = 9;
    double bilateralFilterSigmaColor = 75.0;
    double bilateralFilterSigmaSpace = 75.0;

    cv::dnn::Net yoloNet;
    std::vector<std::string> yoloClassNames;

    bool applyCLAHE = true;
    bool applyBilateral = true;
    bool applyYOLO = true;
};
