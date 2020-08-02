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
    int file_image_quality;

    bool copy_to_clipboard = true;

    QString shortcut_drag;
    QString shortcut_resize_ratio;
};


#endif //SCREEN_SNIPE_SETTING_VALUES_H
