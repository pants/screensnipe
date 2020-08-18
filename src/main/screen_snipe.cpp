#include "screen_snipe.h"

ScreenSnipe::ScreenSnipe(QApplication *t_application) {
    this->m_application = t_application;

    auto icon = QIcon("screensnipe_icon.png");

    this->m_application->setWindowIcon(icon);

    this->m_tray_icon = new QSystemTrayIcon(this);
    this->m_tray_icon->setIcon(icon);

    auto context_menu = createTrayMenu();
    this->m_tray_icon->setContextMenu(context_menu);
    this->m_tray_icon->show();


    QObject::connect(m_capture_window, &CaptureWindow::selectionMade, this, &ScreenSnipe::onSelectionMade);
}

void ScreenSnipe::setupHotkeys() {
    m_screenshot_hotkey = new QHotkey(QKeySequence(k_settings.screenshot_hotkey), true, m_application);

    QObject::connect(m_screenshot_hotkey, &QHotkey::activated, qApp, [&]() {
        this->takeScreenshot();
    });

    auto activeMonitorSequence = QKeySequence(k_settings.screenshot_active_monitor_hotkey);
    m_screenshot_active_monitor_hotkey = new QHotkey(activeMonitorSequence, true, m_application);
    QObject::connect(m_screenshot_active_monitor_hotkey, &QHotkey::activated, qApp, [&]() {
        this->takeScreenshotOfActiveMonitor();
    });
}

void ScreenSnipe::takeScreenshot() {
    m_last_screenshot = m_screen_capturer->captureEntireDesktop();
    m_capture_window->displayCaptureWindow(m_last_screenshot, m_screen_capturer->getScreenDimensions());
}

void ScreenSnipe::takeScreenshotOfActiveMonitor() {
    QScreen *activeScreen;
    if (m_screen_capturer->getActiveScreen(activeScreen)) {
        m_last_screenshot = m_screen_capturer->captureMonitor(activeScreen);
        onSelectionMade(QRect());
    } else {
        qDebug() << "Failed to get active screen..";
    }
}

void ScreenSnipe::onSelectionMade(QRect t_selection) {
    QPixmap screenshot = (t_selection == QRect()) ? m_last_screenshot : this->m_last_screenshot.copy(t_selection);

    if (k_settings.copy_to_clipboard) {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setPixmap(screenshot);
    }

    if (k_settings.file_save) {
        auto current_time = QDateTime::currentDateTime();

        auto path = k_settings.file_path;
        auto filename = current_time.toString(k_settings.file_name);
        auto format = k_settings.file_format;

        auto fullPath = path + "/" + filename + "." + format;

        auto directories = fullPath.left(fullPath.lastIndexOf("/"));
        auto directory = QDir(directories);

        if (!directory.exists()) {
            directory.mkpath(".");
        }

        qDebug() << "Saving to file: " << fullPath;
        screenshot.save(fullPath, format.toStdString().c_str(), k_settings.file_image_quality);
        m_last_screenshot = QPixmap();
    }
}

QMenu *ScreenSnipe::createTrayMenu() {
    auto quit_action = new QAction("&Quit", this);
    connect(quit_action, &QAction::triggered, this, &ScreenSnipe::exit);

    QMenu *menu = new QMenu();
    menu->addAction("Settings");
    menu->addSeparator();
    menu->addAction(quit_action);

    return menu;
}

void ScreenSnipe::exit() {
    //Manually unregister hotkeys to prevent error logs
    m_screenshot_hotkey->setRegistered(false);

    QCoreApplication::exit(0);
}
