#pragma once

#include <QString>
#include <QSettings>

class AppConfig {
public:
    static AppConfig& instance();

    void load();

    QVariant value(const QString& key, const QVariant& defaultValue = {}) const;

    QString get_mode() const;
    QString get_path() const;
    int get_cameraId() const;
    double get_confidenceThreshold() const;
    double get_claheClipLimit() const;
    int get_claheTileGridSize() const;
    int get_bilateralFilterDiameter() const;
    double get_bilateralFilterSigmaColor() const;
    double get_bilateralFilterSigmaSpace() const;
    bool get_loop() const;
    int get_width() const;
    int get_height() const;

private:
    AppConfig(); // private constructor (singleton)
    QSettings settings;

    QString mode;
    QString path;
    int cameraId;
    double confidenceThreshold;
    double claheClipLimit;
    int claheTileGridSize;
    int bilateralFilterDiameter;
    double bilateralFilterSigmaColor;
    double bilateralFilterSigmaSpace;
    bool loop;
    int width;
    int height;
};
