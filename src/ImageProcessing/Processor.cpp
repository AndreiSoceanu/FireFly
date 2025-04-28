#include "Processor.h"
#include "AppConfig.h"
#include <QDebug>

#define YOLO_CFG "Resources/models/yolo/yolov4-tiny.cfg"
#define YOLO_WGH "Resources/models/yolo/yolov4-tiny.weights"

Processor::Processor() {
    confidenceThreshold = AppConfig::instance().get_confidenceThreshold();
    claheClipLimit = AppConfig::instance().get_claheClipLimit();
    claheTileGridSize = AppConfig::instance().get_claheTileGridSize();
    bilateralFilterDiameter = AppConfig::instance().get_bilateralFilterDiameter();
    bilateralFilterSigmaColor = AppConfig::instance().get_bilateralFilterSigmaColor();
    bilateralFilterSigmaSpace = AppConfig::instance().get_bilateralFilterSigmaSpace();

    // Load YOLO class names
    std::ifstream ifs("Resources/models/yolo/coco.names");
    std::string line;
    while (std::getline(ifs, line)) {
        yoloClassNames.push_back(line);
    }

    // Load YOLO model
    yoloNet = cv::dnn::readNetFromDarknet(
        YOLO_CFG,
        YOLO_WGH
    );
    yoloNet.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    yoloNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
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
    if(applyCLAHE){
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(claheClipLimit, cv::Size(claheTileGridSize, claheTileGridSize));
        clahe->apply(labChannels[0], labChannels[0]);
    }
    
    // Apply bilateral filter to the L (lightness) channel
    if(applyBilateral){
        cv::Mat filteredL;
        cv::bilateralFilter(labChannels[0], filteredL, bilateralFilterDiameter, bilateralFilterSigmaColor, bilateralFilterSigmaSpace);  
        // Merge the filtered L channel with the original A and B channels
        labChannels[0] = filteredL;  
    }

    // Merge and convert back to BGR
    cv::merge(labChannels, lab);
    cv::cvtColor(lab, output, cv::COLOR_Lab2BGR);

     // Call the new detectHumans function
     if(applyYOLO){
        detectHumans(output);
     }

    return output;
}

void Processor::detectHumans(cv::Mat& output) {
    // YOLO Input Blob
    cv::Mat blob;
    cv::dnn::blobFromImage(output, blob, 1 / 255.0, cv::Size(416, 416), cv::Scalar(), true, false);
    yoloNet.setInput(blob);

    std::vector<cv::Mat> outputs;
    yoloNet.forward(outputs, yoloNet.getUnconnectedOutLayersNames());

    float confThreshold = 0.5;
    for (const auto& yoloOutput : outputs) {
        for (int i = 0; i < yoloOutput.rows; i++) {
            const float* data = yoloOutput.ptr<float>(i);
            float confidence = data[4];
            if (confidence > confThreshold) {
                int classId = std::max_element(data + 5, data + yoloOutput.cols) - (data + 5);
                float classScore = data[5 + classId];
                if (classScore > confThreshold && yoloClassNames[classId] == "person") {
                    int centerX = static_cast<int>(data[0] * output.cols);
                    int centerY = static_cast<int>(data[1] * output.rows);
                    int width = static_cast<int>(data[2] * output.cols);
                    int height = static_cast<int>(data[3] * output.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    cv::rectangle(output, cv::Rect(left, top, width, height), cv::Scalar(0, 255, 0), 2);
                    cv::putText(output, "Human", cv::Point(left, top - 10),
                                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
                }
            }
        }
    }
}

void Processor::setApplyClahe(bool on){
    applyCLAHE = on;
}

void Processor::setApplyBilateral(bool on){
    applyBilateral = on;
}

void Processor::setApplyYolo(bool on){
    applyYOLO = on;
}
