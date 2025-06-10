#include "AppConfig.h"
#include <QDebug>

AppConfig::AppConfig()
    : settings("config/config.ini", QSettings::IniFormat)
{
    // Load settings from the config file
    mode = settings.value("mode/type", "webcam").toString();
    path = settings.value("mode/path", "").toString();
    loop = settings.value("video/loop", true).toBool();
    cameraId = settings.value("camera/id", 0).toInt();
    confidenceThreshold = settings.value("yolo/confidence", 0.5).toDouble();
    claheClipLimit = settings.value("clahe/clipLimit", 2.0).toDouble();
    claheTileGridSize = settings.value("clahe/tileGridSize", 8).toInt();
    bilateralFilterDiameter = settings.value("filter/diameter", 9).toInt();
    bilateralFilterSigmaColor = settings.value("filter/sigmaColor", 75.0).toDouble();
    bilateralFilterSigmaSpace = settings.value("filter/sigmaSpace", 75.0).toDouble();
    width = settings.value("display/width", 640).toInt();
    height = settings.value("display/height", 480).toInt();

    // Print loaded settings for debugging
    qDebug() << "Loaded settings:";
    qDebug() << "Mode:" << mode;
    qDebug() << "Path:" << path;
    qDebug() << "Loop:" << loop;
    qDebug() << "Camera ID:" << cameraId;
    qDebug() << "Confidence Threshold:" << confidenceThreshold;
    qDebug() << "CLAHE Clip Limit:" << claheClipLimit;
    qDebug() << "CLAHE Tile Grid Size:" << claheTileGridSize;
    qDebug() << "Bilateral Filter Diameter:" << bilateralFilterDiameter;
    qDebug() << "Bilateral Filter Sigma Color:" << bilateralFilterSigmaColor;
    qDebug() << "Bilateral Filter Sigma Space:" << bilateralFilterSigmaSpace;
    qDebug() << "Display Width:" << width;
    qDebug() << "Display Height:" << height;
    qDebug() << "--------------------------------";
}

AppConfig& AppConfig::instance() {
    static AppConfig instance;
    return instance;
}

QVariant AppConfig::value(const QString& key, const QVariant& defaultValue) const {
    return settings.value(key, defaultValue);
}

QString AppConfig::get_mode() const { return mode; }
QString AppConfig::get_path() const { return path; }
int AppConfig::get_cameraId() const { return cameraId; }
double AppConfig::get_confidenceThreshold() const { return confidenceThreshold; }
double AppConfig::get_claheClipLimit() const { return claheClipLimit; }
int AppConfig::get_claheTileGridSize() const { return claheTileGridSize; }
int AppConfig::get_bilateralFilterDiameter() const { return bilateralFilterDiameter; }
double AppConfig::get_bilateralFilterSigmaColor() const { return bilateralFilterSigmaColor; }
double AppConfig::get_bilateralFilterSigmaSpace() const { return bilateralFilterSigmaSpace; }
bool AppConfig::get_loop() const { return loop; }
int AppConfig::get_width() const { return width; }
int AppConfig::get_height() const { return height; }



