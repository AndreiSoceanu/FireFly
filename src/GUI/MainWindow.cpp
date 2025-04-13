#include "MainWindow.h"
#include "Capture.h"
#include "Processor.h"

#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      videoLabel(new QLabel(this))
{
    setWindowTitle("FireFly");
    // resize(800, 600);
    videoLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    videoLabel->setAlignment(Qt::AlignCenter);
    // videoLabel->setMinimumSize(640, 480);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->addWidget(videoLabel);
    central->setLayout(layout);
    setCentralWidget(central);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
}

MainWindow& MainWindow::instance() {
    static MainWindow instance;
    return instance;
}

void MainWindow::initialize() {
    // Prime the camera — get a valid first frame
    cv::Mat frame;
    for (int i = 0; i < 5; ++i) {  // Try a few times, just in case
        frame = Capture::instance().getFrame();
        if (!frame.empty()) break;
    }

    if (!frame.empty()) {
        resize(frame.cols, frame.rows + 40); // Resize main window
        videoLabel->setFixedSize(frame.cols, frame.rows);
    } else {
        qWarning("⚠️ Failed to grab initial frame for layout");
    }
}


void MainWindow::start() {
    QTimer::singleShot(0, this, &MainWindow::updateFrame);
}

void MainWindow::updateFrame() {
    cv::Mat input = Capture::instance().getFrame();
    cv::Mat output = Processor::instance().process(input);

    if (output.empty()) return;

    cv::cvtColor(output, output, cv::COLOR_BGR2RGB);
    QImage img(output.data, output.cols, output.rows, output.step, QImage::Format_RGB888);
    videoLabel->setPixmap(QPixmap::fromImage(img));
    QTimer::singleShot(0, this, &MainWindow::updateFrame);
}
