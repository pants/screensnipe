#ifndef SCREENSNIPE_CAPTUREWINDOW_H
#define SCREENSNIPE_CAPTUREWINDOW_H

#include "../main/screen_capturer.h"
#include "../main/conf/setting_values.h"
#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QDebug>

class CaptureWindow : public QWidget {
Q_OBJECT
public:
    explicit CaptureWindow(SettingValues t_settings);

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

    SettingValues m_settings;

    bool m_started_selection = false;
    bool m_resize_selection_pressed = false;
    bool m_move_selection_pressed = false;

    void onInputEvent(bool t_pressed, QKeyEvent *t_event);
    void makeSelection(QPoint t_end_pos);
    void moveSelection();

    QString keyToText(int t_key);

signals:
    void selectionMade(QRect t_selection);
};


#endif //SCREENSNIPE_CAPTUREWINDOW_H
