#include "screen_capturer.h"

QRect ScreenCapturer::getScreenDimensions() {
    auto screens = QApplication::screens();

    int max_width = 0, max_height = 0;

    for(auto screen : screens){
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

    QScreen* primary_screen = QApplication::primaryScreen();
    QPixmap screenshot = primary_screen->grabWindow(desktop->winId(), 0, 0, screen_geometry.width(), screen_geometry.height());

    return screenshot;
}

ScreenCapturer::ScreenCapturer() {

}