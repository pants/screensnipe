#include "settings_loader.h"

SettingsLoader::SettingsLoader() {
    loadSettings();
    m_settings->sync();
}

void SettingsLoader::loadSettings() {
    screenshot_hotkey = value("screenshot_hotkey", "ctrl+shift+4").toString();
    copy_to_clipboard = value("copy_to_clipboard", true).toBool();

    m_settings->beginGroup("file");
    file_save = value("save_screenshot", true).toBool();
    file_name = value("name", "yyyy-MM/yyyy-MM-dd_HH-mm-ss").toString();
    file_format = value("format", "jpg").toString();
    file_path = value("path", QDir::homePath() + "/Pictures/screensnipe").toString();
    file_image_quality = value("image_quality", 80).toInt();
    m_settings->endGroup();

    m_settings->sync();
}

QVariant SettingsLoader::value(const QString &key, const QVariant &defaultValue) {
    auto value = m_settings->value(key, defaultValue);
    m_settings->setValue(key, value);
    return value;
}
