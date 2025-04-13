#pragma once

#include <QString>
#include <QSettings>

class AppConfig {
public:
    static AppConfig& instance();

    void load();
    int cameraId() const;
    float confidenceThreshold() const;

    QVariant value(const QString& key, const QVariant& defaultValue = {}) const;


private:
    AppConfig(); // private constructor (singleton)
    QSettings settings;
};
