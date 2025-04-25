#include "Processor.h"
#include "AppConfig.h"
#include <QDebug>

Processor::Processor() {
    confidenceThreshold = AppConfig::instance().get_confidenceThreshold();
    claheClipLimit = AppConfig::instance().get_claheClipLimit();
    claheTileGridSize = AppConfig::instance().get_claheTileGridSize();
    bilateralFilterDiameter = AppConfig::instance().get_bilateralFilterDiameter();
    bilateralFilterSigmaColor = AppConfig::instance().get_bilateralFilterSigmaColor();
    bilateralFilterSigmaSpace = AppConfig::instance().get_bilateralFilterSigmaSpace();
}

Processor& Processor::instance() {
    static Processor instance;
    return instance;
}

void Processor::initialize() {
    // Later: load YOLO or other models
}

cv::Mat Processor::process(const cv::Mat& input) {
    if (input.empty()) return {};

    cv::Mat output;
    input.copyTo(output);
    // Convert to LAB color space
    cv::Mat lab;
    cv::cvtColor(output, lab, cv::COLOR_BGR2Lab);

    // Split LAB into 3 channels
    std::vector<cv::Mat> labChannels(3);
    cv::split(lab, labChannels);
    
    // Apply CLAHE to the L (lightness) channel
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(claheClipLimit, cv::Size(claheTileGridSize, claheTileGridSize));
    clahe->apply(labChannels[0], labChannels[0]);
    // Apply bilateral filter to the L (lightness) channel
    cv::Mat filteredL;
    cv::bilateralFilter(labChannels[0], filteredL, bilateralFilterDiameter, bilateralFilterSigmaColor, bilateralFilterSigmaSpace);

    // Merge the filtered L channel with the original A and B channels
    labChannels[0] = filteredL;

    // Merge and convert back to BGR
    cv::merge(labChannels, lab);
    cv::cvtColor(lab, output, cv::COLOR_Lab2BGR);

    return output;
}
