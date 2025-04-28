#include "MainWindow.h"
#include "AppConfig.h"
#include "Capture.h"
#include "Processor.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), videoLabel(new QLabel(this)), timer(new QTimer(this)),
      claheCheckBox(new QCheckBox("Apply CLAHE", this)),
      bilateralCheckBox(new QCheckBox("Apply Bilateral Filter", this)),
      yoloCheckBox(new QCheckBox("Apply YOLO Detection", this))
{
    setWindowTitle("FireFly");

    videoLabel->setAlignment(Qt::AlignCenter);
    videoLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->addWidget(videoLabel);
    layout->addWidget(claheCheckBox);
    layout->addWidget(bilateralCheckBox);
    layout->addWidget(yoloCheckBox);
    setCentralWidget(central);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    // CONNECT CHECKBOXES TO SLOTS
    connect(claheCheckBox, &QCheckBox::checkStateChanged, this, &MainWindow::toggleCLAHE);
    connect(bilateralCheckBox, &QCheckBox::checkStateChanged, this, &MainWindow::toggleBilateral);
    connect(yoloCheckBox, &QCheckBox::checkStateChanged, this, &MainWindow::toggleYOLO);

    // Set initial states (all ON)
    claheCheckBox->setChecked(true);
    bilateralCheckBox->setChecked(true);
    yoloCheckBox->setChecked(true);

}

MainWindow& MainWindow::instance() {
    static MainWindow instance;
    return instance;
}

void MainWindow::initialize() {
    if (!Capture::instance().isReady()) {
        qCritical("❌ Capture not ready during MainWindow::initialize");
        return;
    }

    // Try to grab a first frame to determine size
    cv::Mat first = Capture::instance().getFrame();
    if (!first.empty()) {
        videoLabel->setFixedSize(first.cols, first.rows);
        resize(first.cols, first.rows + 40);  // extra room for window borders
    } else {
        qWarning("⚠️ Initial frame is empty.");
    }
}

void MainWindow::start() {
    if (!Capture::instance().isReady()) {
        qWarning("❌ Capture not ready. Aborting start.");
        return;
    }

    QString mode = AppConfig::instance().get_mode();

    if (mode == "image") {
        updateFrame();  // just display once
    } else {
        timer->start(30);  // ~33 FPS for webcam/video
    }
}

void MainWindow::updateFrame() {
    cv::Mat input = Capture::instance().getFrame();

    if (input.empty()) {
        qWarning("⚠️ Frame is empty.");
        return;
    }

    cv::Mat processed = Processor::instance().process(input);
    if (processed.empty()) return;

    cv::cvtColor(processed, processed, cv::COLOR_BGR2RGB);  // Qt needs RGB

    QImage img(processed.data, processed.cols, processed.rows, processed.step, QImage::Format_RGB888);
    videoLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::toggleCLAHE(int state) {
    bool enabled = (state == Qt::Checked);
    Processor::instance().setApplyClahe(enabled);
}

void MainWindow::toggleBilateral(int state) {
    bool enabled = (state == Qt::Checked);
    Processor::instance().setApplyBilateral(enabled);
}

void MainWindow::toggleYOLO(int state) {
    bool enabled = (state == Qt::Checked);
    Processor::instance().setApplyYolo(enabled);
}

