#ifndef SCREEN_SNIPE_SETTING_VALUES_H
#define SCREEN_SNIPE_SETTING_VALUES_H

#include <QString>

class SettingValues {
public:
    QString screenshot_hotkey;

    bool file_save;
    QString file_path;
    QString file_name;
    QString file_format;

    bool copy_to_clipboard = true;
};


#endif //SCREEN_SNIPE_SETTING_VALUES_H
