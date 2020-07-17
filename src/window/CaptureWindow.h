#ifndef SCREENSNIPE_CAPTUREWINDOW_H
#define SCREENSNIPE_CAPTUREWINDOW_H

#include "../main/ScreenCapturer.h"
#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>

class CaptureWindow : public QWidget {
Q_OBJECT
public:
    explicit CaptureWindow();

    void displayCaptureWindow(const QPixmap &t_screenshot, QRect t_screenGeometry);

protected:
    void paintEvent(QPaintEvent *t_event) override;

    void mousePressEvent(QMouseEvent *t_event) override;

    void mouseReleaseEvent(QMouseEvent *t_event) override;

    void mouseMoveEvent(QMouseEvent *t_event) override;

private:
    QPoint m_start_mouse_pos = {0, 0};
    bool m_started_selection = false;

signals:
    void selectionMade(QRect t_selection);
};


#endif //SCREENSNIPE_CAPTUREWINDOW_H
