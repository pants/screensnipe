#include "ScreenSnipe.h"

ScreenSnipe::ScreenSnipe(QApplication* t_application) {
    this->m_application = t_application;
    QObject::connect(m_capture_window, &CaptureWindow::selectionMade, this, &ScreenSnipe::onSelectionMade);
}

void ScreenSnipe::setupHotkeys() {
    m_screenshot_hotkey = new QHotkey(QKeySequence("ctrl+shift+4"), true, m_application);

    QObject::connect(m_screenshot_hotkey, &QHotkey::activated, qApp, [&](){
        this->takeScreenshot();
    });
}

void ScreenSnipe::takeScreenshot() {
    m_last_screenshot = m_screen_capturer->captureEntireDesktop();
    m_capture_window->displayCaptureWindow(m_last_screenshot, m_screen_capturer->getScreenDimensions());
}

void ScreenSnipe::onSelectionMade(QRect t_selection) {
    QPixmap screenshot = this->m_last_screenshot.copy(t_selection);
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setPixmap(screenshot);
}
