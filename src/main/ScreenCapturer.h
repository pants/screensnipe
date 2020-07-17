#ifndef SCREENSNIPE_SCREENCAPTURER_H
#define SCREENSNIPE_SCREENCAPTURER_H

#include <QApplication>
#include <QScreen>
#include <QRect>
#include <QPixmap>
#include <QDesktopWidget>

class ScreenCapturer {
public:
    explicit ScreenCapturer();

    QPixmap captureEntireDesktop();

    //Gets the combined screen dimensions across multiple monitors
    QRect getScreenDimensions();
};


#endif //SCREENSNIPE_SCREENCAPTURER_H
