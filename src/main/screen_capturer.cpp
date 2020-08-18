#include "screen_capturer.h"

QRect ScreenCapturer::getScreenDimensions() {
    auto screens = QApplication::screens();

    int max_width = 0, max_height = 0;

    for (auto screen : screens) {
        auto box = screen->geometry();
        auto real_width = box.x() + box.width();
        auto real_height = box.y() + box.height();

        max_width = std::max(max_width, real_width);
        max_height = std::max(max_height, real_height);
    }

    return {0, 0, max_width, max_height};
}

QPixmap ScreenCapturer::captureEntireDesktop() {
    QRect screen_geometry = getScreenDimensions();
    QDesktopWidget *desktop = QApplication::desktop();

    QScreen *primary_screen = QApplication::primaryScreen();
    QPixmap screenshot = primary_screen->grabWindow(desktop->winId(), 0, 0, screen_geometry.width(),
                                                    screen_geometry.height());

    return screenshot;
}

QPixmap ScreenCapturer::captureMonitor(QScreen *t_screen) {
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen_geometry = t_screen->geometry();

    auto real_width = screen_geometry.width();
    auto real_height = screen_geometry.height();

    return t_screen->grabWindow(desktop->winId(), screen_geometry.x(), screen_geometry.y(), real_width, real_height);
}

bool ScreenCapturer::getActiveScreen(QScreen *&output) {
    auto screens = QApplication::screens();
    auto cursor_pos = QCursor::pos();

    for (auto screen : screens) {
        auto box = screen->geometry();
        auto real_width = box.x() + box.width();
        auto real_height = box.y() + box.height();

        auto inUpper = cursor_pos.x() > box.x() && cursor_pos.y() > box.y();
        auto inBottom = cursor_pos.x() < real_width && cursor_pos.y() < box.height();
        if (inUpper && inBottom) {
            qDebug() << "Found active screen: " << box;
            output = screen;
            return true;
        }
    }

    return false;
}
