#ifndef SCREENSNIPE_SCREENCAPTURER_H
#define SCREENSNIPE_SCREENCAPTURER_H

#include <QApplication>
#include <QScreen>
#include <QRect>
#include <QPixmap>
#include <QDesktopWidget>
#include <QCursor>
#include <QDebug>

class ScreenCapturer {
public:
    bool getActiveScreen(QScreen *&output);

    QPixmap captureEntireDesktop();
    QPixmap captureMonitor(QScreen *t_screen);

    //Gets the combined screen dimensions across multiple monitors
    QRect getScreenDimensions();
};


#endif //SCREENSNIPE_SCREENCAPTURER_H
