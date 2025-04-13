#include "Processor.h"
#include "AppConfig.h"

Processor::Processor() {
    confidenceThreshold = AppConfig::instance().value("processing/confidence", 0.5).toFloat();
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

    return output;
}
