#include <gtest/gtest.h>
#include "AppConfig.h"
#include "Capture.h"
#include "Processor.h"

TEST(AppConfigTest, DefaultValuesLoadCorrectly) {
    auto& config = AppConfig::instance();

    EXPECT_EQ(config.get_width(), 640);
    EXPECT_EQ(config.get_height(), 480);
    EXPECT_DOUBLE_EQ(config.get_claheClipLimit(), 3.0);
    EXPECT_EQ(config.get_claheTileGridSize(), 4);
    EXPECT_TRUE(config.get_loop()); // depending on your ini config
}

TEST(CaptureTest, WebcamInitializationWorksIfAvailable) {
    auto& capture = Capture::instance();

    // Initialize with current config (webcam mode)
    bool ok = capture.initialize();

    // This might be true or false depending on hardware, so we check logic, not camera
    EXPECT_EQ(capture.isReady(), ok);
}


TEST(ProcessorTest, ClaheBrightensDarkImage) {
    // Create dark gray image
    cv::Mat dark(100, 100, CV_8UC3, cv::Scalar(20, 20, 20));

    auto& proc = Processor::instance();
    proc.setApplyClahe(true);
    proc.setApplyBilateral(false);
    proc.setApplyYolo(false);

    cv::Mat out = proc.process(dark);

    // Calculate mean brightness
    cv::Scalar meanOut = cv::mean(out);
    EXPECT_GT(meanOut[0], 20);  // Expect some brightening
}

TEST(SingletonTest, CaptureAlwaysReturnsSameInstance) {
    auto& a = Capture::instance();
    auto& b = Capture::instance();
    EXPECT_EQ(&a, &b);
}

TEST(ProcessorTest, BilateralPreservesEdgeContrast) {
    // Create 2-block grayscale image: left=50, right=200
    cv::Mat sharpEdge(100, 100, CV_8UC3);
    sharpEdge.setTo(cv::Scalar(50, 50, 50));
    sharpEdge.colRange(50, 100).setTo(cv::Scalar(200, 200, 200));

    auto& proc = Processor::instance();
    proc.setApplyClahe(false);
    proc.setApplyBilateral(true);
    proc.setApplyYolo(false);

    cv::Mat output = proc.process(sharpEdge);

    // Sample pixels from both sides
    cv::Vec3b left = output.at<cv::Vec3b>(50, 25);
    cv::Vec3b right = output.at<cv::Vec3b>(50, 75);

    int contrast = std::abs(left[0] - right[0]);
    EXPECT_GT(contrast, 100);  // Expect high edge contrast preserved
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }
  