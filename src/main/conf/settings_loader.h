#ifndef SCREEN_SNIPE_SETTINGS_LOADER_H
#define SCREEN_SNIPE_SETTINGS_LOADER_H

#include "setting_values.h"
#include <QSettings>
#include <QVariant>
#include <QDir>

class SettingsLoader : public SettingValues {
public:
    explicit SettingsLoader();

    void loadSettings();

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());

private:
    QSettings *m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                          "screensnipe", "screensnipe");
};


#endif //SCREEN_SNIPE_SETTINGS_LOADER_H
