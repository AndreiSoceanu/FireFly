#include "MainWindow.h"
#include "Capture.h"
#include "Processor.h"

#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      videoLabel(new QLabel(this)),
      timer(new QTimer(this))
{
    setWindowTitle("FireFly");
    resize(800, 600);

    videoLabel->setAlignment(Qt::AlignCenter);
    videoLabel->setMinimumSize(640, 480);

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
    // Capture and Processor should already be initialized externally
}

void MainWindow::start() {
    timer->start(30);  // ~33 FPS
}

void MainWindow::updateFrame() {
    cv::Mat input = Capture::instance().getFrame();
    cv::Mat output = Processor::instance().process(input);

    if (output.empty()) return;

    cv::cvtColor(output, output, cv::COLOR_BGR2RGB);
    QImage img(output.data, output.cols, output.rows, output.step, QImage::Format_RGB888);
    videoLabel->setPixmap(QPixmap::fromImage(img));
}
