#include "capture_window.h"

#include <utility>

CaptureWindow::CaptureWindow(SettingValues t_settings) : m_settings(std::move(t_settings)) {
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::WindowType::X11BypassWindowManagerHint);
}

void CaptureWindow::displayCaptureWindow(const QPixmap &t_screenshot, QRect t_screenGeometry) {
    this->m_start_mouse_pos = {0, 0};
    this->m_started_selection = false;

    QPalette palette;
    palette.setBrush(QPalette::Background, t_screenshot);
    this->setPalette(palette);

    this->setGeometry(t_screenGeometry);

    this->show();

    //When using X11BypassWindowManagerHint this allows keyboard input into the window, needs to be called after show()
    this->activateWindow();
}

void CaptureWindow::makeSelection(QPoint t_end_pos) {
    int start_x = m_start_mouse_pos.x();
    int start_y = m_start_mouse_pos.y();
    int end_x = t_end_pos.x();
    int end_y = t_end_pos.y();

    int min_x = std::min(start_x, end_x);
    int max_x = std::max(start_x, end_x);

    int min_y = std::min(start_y, end_y);
    int max_y = std::max(start_y, end_y);

    int width = max_x - min_x;
    int height = max_y - min_y;

    this->hide();

    QRect selection = {min_x, min_y, width, height};
    emit selectionMade(selection);
}

void CaptureWindow::paintEvent(QPaintEvent *t_event) {
    QPainter painter(this);

    //Background tint
    painter.fillRect(0, 0, this->width(), this->height(), QColor(0, 0, 0, 50));

    if (!m_started_selection) return;

    QPoint mousePos = QCursor::pos();

    int draw_x = m_start_mouse_pos.x();
    int draw_y = m_start_mouse_pos.y();

    int draw_height = mousePos.y() - draw_y;
    //If ctrl is pressed, keep a 1:1 aspect ratio
    int draw_width = m_resize_selection_pressed ? draw_height : mousePos.x() - draw_x;

    int outline_offset_x = (draw_width < 0) ? -2 : 2;
    int outline_offset_y = (draw_height < 0) ? -2 : 2;

    //Selection outline
    painter.fillRect(
            draw_x - outline_offset_x,
            draw_y - outline_offset_y,
            draw_width + (outline_offset_x * 2),
            draw_height + (outline_offset_y * 2),
            QColor(120, 120, 120, 200)
    );

    painter.eraseRect(draw_x, draw_y, draw_width, draw_height);
}

void CaptureWindow::mousePressEvent(QMouseEvent *t_event) {
    m_start_mouse_pos = {t_event->x(), t_event->y()};
    m_started_selection = true;
}

void CaptureWindow::mouseReleaseEvent(QMouseEvent *t_event) {
    //Cancels the drag selection when the user right clicks
    if (t_event->button() == Qt::MouseButton::RightButton) {
        m_started_selection = false;
        repaint();
        return;
    }

    bool is_mouse_left = t_event->button() == Qt::MouseButton::LeftButton;

    if (!m_started_selection || !is_mouse_left) {
        return;
    }

    makeSelection(t_event->pos());
}

void CaptureWindow::mouseMoveEvent(QMouseEvent *t_event) {
    if (m_move_selection_pressed) {
        moveSelection();
    }
    repaint();
}

void CaptureWindow::keyPressEvent(QKeyEvent *t_event) {
    bool is_pressed = true;
    onInputEvent(is_pressed, t_event);
}

void CaptureWindow::keyReleaseEvent(QKeyEvent *t_event) {
    bool not_pressed = false;
    onInputEvent(not_pressed, t_event);
}

void CaptureWindow::onInputEvent(bool t_pressed, QKeyEvent *t_event) {
    auto key = keyToText(t_event->key());

    if (key == m_settings.shortcut_drag) {
        this->m_move_selection_pressed = t_pressed;
        this->m_last_mouse_pos = QCursor::pos();
    } else if (key == m_settings.shortcut_resize_ratio) {
        this->m_resize_selection_pressed = t_pressed;
    } else if (key == "esc") {
        this->hide();
    }
}

QString CaptureWindow::keyToText(int t_key) {
    switch (t_key) {
        case Qt::Key::Key_Control:
            return "ctrl";
        case Qt::Key::Key_Shift:
            return "shift";
        case Qt::Key::Key_Alt:
            return "alt";
        default:
            return QKeySequence(t_key).toString().toLower();
    }
}

void CaptureWindow::moveSelection() {
    QPoint current_pos = QCursor::pos();
    QPoint cursor_difference = current_pos - m_last_mouse_pos;
    this->m_start_mouse_pos += cursor_difference;
    this->m_last_mouse_pos = current_pos;
}
