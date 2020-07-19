#include "screen_snipe.h"

ScreenSnipe::ScreenSnipe(QApplication *t_application) {
    this->m_application = t_application;
    QObject::connect(m_capture_window, &CaptureWindow::selectionMade, this, &ScreenSnipe::onSelectionMade);
}

void ScreenSnipe::setupHotkeys() {
    m_screenshot_hotkey = new QHotkey(QKeySequence(k_settings.screenshot_hotkey), true, m_application);

    QObject::connect(m_screenshot_hotkey, &QHotkey::activated, qApp, [&]() {
        this->takeScreenshot();
    });
}

void ScreenSnipe::takeScreenshot() {
    m_last_screenshot = m_screen_capturer->captureEntireDesktop();
    m_capture_window->displayCaptureWindow(m_last_screenshot, m_screen_capturer->getScreenDimensions());
}

void ScreenSnipe::onSelectionMade(QRect t_selection) {
    QPixmap screenshot = this->m_last_screenshot.copy(t_selection);

    if (k_settings.copy_to_clipboard) {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setPixmap(screenshot);
    }

    if(k_settings.file_save){
        auto current_time = QDateTime::currentDateTime();

        auto path = k_settings.file_path;
        auto filename = current_time.toString(k_settings.file_name);
        auto format = k_settings.file_format;

        auto fullPath = path + "/" + filename + "." + format;

        auto directories = fullPath.left(fullPath.lastIndexOf("/"));
        auto directory = QDir(directories);

        if(!directory.exists()){
            directory.mkpath(".");
        }

        qDebug() << "Saving to file: " << fullPath;
        screenshot.save(fullPath, format.toStdString().c_str());
    }
}
