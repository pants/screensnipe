#ifndef SCREENSNIPE_SCREENSNIPE_H
#define SCREENSNIPE_SCREENSNIPE_H

#include "screen_capturer.h"
#include "conf/settings_loader.h"
#include "../window/capture_window.h"
#include <QApplication>
#include <QHotkey>
#include <QClipboard>
#include <QDir>
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QMenu>

class ScreenSnipe : public QObject {
Q_OBJECT
public:
    explicit ScreenSnipe(QApplication *t_application);

    void setupHotkeys();

protected:
    void onSelectionMade(QRect t_selection);

private:
    const SettingsLoader k_settings = SettingsLoader();

    ScreenCapturer *m_screen_capturer = new ScreenCapturer();
    CaptureWindow *m_capture_window = new CaptureWindow();

    QApplication *m_application = nullptr;
    QHotkey *m_screenshot_hotkey = nullptr;

    QPixmap m_last_screenshot = QPixmap();

    QSystemTrayIcon* m_tray_icon;

    void exit();
    QMenu* createTrayMenu();
    void takeScreenshot();
};


#endif //SCREENSNIPE_SCREENSNIPE_H
