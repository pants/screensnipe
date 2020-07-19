#include "capture_window.h"

CaptureWindow::CaptureWindow() {
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

    int drawX = m_start_mouse_pos.x();
    int drawY = m_start_mouse_pos.y();

    int drawWidth = mousePos.x() - drawX;
    int drawHeight = mousePos.y() - drawY;

    int outlineOffsetX = (drawWidth < 0) ? -2 : 2;
    int outlineOffsetY = (drawHeight < 0) ? -2 : 2;

    //Selection outline
    painter.fillRect(
            drawX - outlineOffsetX,
            drawY - outlineOffsetY,
            drawWidth + (outlineOffsetX * 2),
            drawHeight + (outlineOffsetY * 2),
            QColor(120, 120, 120, 200)
    );

    painter.eraseRect(drawX, drawY, drawWidth, drawHeight);
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
    repaint();
}

void CaptureWindow::keyReleaseEvent(QKeyEvent *t_event) {
    if (t_event->key() == Qt::Key::Key_Escape) {
        this->hide();
    }
}