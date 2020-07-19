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

void CaptureWindow::paintEvent(QPaintEvent *t_event) {
    QPainter painter(this);

    //Background tint
    painter.fillRect(0, 0, this->width(), this->height(), QColor(0, 0, 0, 50));

    if(!m_started_selection) return;

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
            QColor(100, 100, 100, 100)
    );

    painter.eraseRect(drawX, drawY, drawWidth, drawHeight);
}

void CaptureWindow::mousePressEvent(QMouseEvent *t_event) {
    m_start_mouse_pos = {t_event->x(), t_event->y()};
    m_started_selection = true;
}

void CaptureWindow::mouseReleaseEvent(QMouseEvent *t_event) {
    int selectionX = m_start_mouse_pos.x();
    int selectionY = m_start_mouse_pos.y();

    int width = t_event->x() - selectionX;
    int height = t_event->y() - selectionY;

    this->hide();

    QRect selection = {selectionX, selectionY, width, height};
    emit selectionMade(selection);
}

void CaptureWindow::mouseMoveEvent(QMouseEvent *t_event) {
    repaint();
}

void CaptureWindow::keyReleaseEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key::Key_Escape){
        this->hide();
    }
}