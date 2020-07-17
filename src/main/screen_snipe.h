#ifndef SCREENSNIPE_SCREENSNIPE_H
#define SCREENSNIPE_SCREENSNIPE_H

#include "screen_capturer.h"
#include "../window/capture_window.h"
#include <QApplication>
#include <QHotkey>
#include <QClipboard>

class ScreenSnipe : public QObject {
Q_OBJECT
public:
    explicit ScreenSnipe(QApplication *t_application);

    void setupHotkeys();

protected:
    void onSelectionMade(QRect t_selection);

private:
    ScreenCapturer *m_screen_capturer = new ScreenCapturer();
    CaptureWindow *m_capture_window = new CaptureWindow();

    QApplication *m_application = nullptr;
    QHotkey *m_screenshot_hotkey = nullptr;

    QPixmap m_last_screenshot = QPixmap();

    void takeScreenshot();
};


#endif //SCREENSNIPE_SCREENSNIPE_H
