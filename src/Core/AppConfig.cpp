#include "AppConfig.h"

AppConfig::AppConfig()
    : settings("config/config.ini", QSettings::IniFormat)
{
    // Settings object initialized here
}

AppConfig& AppConfig::instance() {
    static AppConfig instance;
    return instance;
}

void AppConfig::load() {
    // Future: trigger reloading manually if needed
}

int AppConfig::cameraId() const {
    return settings.value("camera/id", 0).toInt();
}

float AppConfig::confidenceThreshold() const {
    return settings.value("yolo/confidence", 0.5).toFloat();
}

QVariant AppConfig::value(const QString& key, const QVariant& defaultValue) const {
    return settings.value(key, defaultValue);
}

