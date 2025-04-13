#include <QApplication>
#include <QDebug>

#include "AppConfig.h"
#include "MainWindow.h"
#include "Capture.h"
#include "Processor.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Load config values (already done implicitly in singleton constructors)
    if (!Capture::instance().initialize()) {
        qCritical("❌ Failed to initialize input capture");
        return -1;
    }

    Processor::instance().initialize();
    MainWindow& window = MainWindow::instance();
    window.initialize();
    window.show();
    window.start();

    return app.exec();
}
