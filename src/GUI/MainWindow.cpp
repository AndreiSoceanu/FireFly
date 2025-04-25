#include "MainWindow.h"
#include "AppConfig.h"
#include "Capture.h"
#include "Processor.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), videoLabel(new QLabel(this)), timer(new QTimer(this))
{
    setWindowTitle("FireFly");

    videoLabel->setAlignment(Qt::AlignCenter);
    videoLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->addWidget(videoLabel);
    setCentralWidget(central);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
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
