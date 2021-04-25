#ifndef GRAPHICS_VIEW_HPP
#define GRAPHICS_VIEW_HPP

#include <QGraphicsView>
#include <QScrollBar>
#include <QMouseEvent>

class graphics_view : public QGraphicsView
{
    Q_OBJECT
private:
    QPoint click_position;
    QPointF last_mouse_position;

    bool panning;
    bool zooming;
    int panning_start_x, panning_start_y;
    QRectF last_rect;

    void zoom(int delta, QPoint position);
    QRectF get_view_rect() const;

public:
    graphics_view();
    ~graphics_view();

    void restore_rect(QRectF rect);

protected:
    inline QRectF get_last_rect() const;
    inline void freeze_current_rect();

    // Events
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
};

inline void graphics_view::freeze_current_rect()
{
    last_rect = get_view_rect();
}

inline QRectF graphics_view::get_last_rect() const
{
    return last_rect;
}

#endif // GRAPHICS_VIEW_HPP
