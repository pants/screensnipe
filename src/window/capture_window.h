#ifndef SCREENSNIPE_CAPTUREWINDOW_H
#define SCREENSNIPE_CAPTUREWINDOW_H

#include "../main/screen_capturer.h"
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

    void keyReleaseEvent(QKeyEvent *t_event) override;

    void keyPressEvent(QKeyEvent *t_event) override;

private:
    QPoint m_start_mouse_pos = {0, 0};
    QPoint m_last_mouse_pos = {0, 0};

    bool m_started_selection = false;
    bool m_shift_press = false;
    bool m_ctrl_pressed = false;

    void onInputEvent(bool t_pressed, QKeyEvent *t_event);
    void makeSelection(QPoint t_end_pos);
    void moveSelection();

signals:
    void selectionMade(QRect t_selection);
};


#endif //SCREENSNIPE_CAPTUREWINDOW_H
