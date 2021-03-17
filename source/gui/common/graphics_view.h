#ifndef GRAPHICS_VIEW_HPP
#define GRAPHICS_VIEW_HPP

#include <QGraphicsView>
#include <QScrollBar>

#include <qt.h>
#include <util.h>

class graphics_view : public QGraphicsView
{
    Q_OBJECT
private:
    QPoint click_position;
    QPointF last_mouse_position;

    bool panning;
    bool zooming;
    int panning_start_x, panning_start_y;

    void zoom( int delta, QPoint position );

public:
    graphics_view( /* args */ );
    ~graphics_view();

protected:
    // Events
    void mousePressEvent( QMouseEvent *event ) override;
    void mouseReleaseEvent( QMouseEvent *event ) override;
    void mouseMoveEvent( QMouseEvent *event ) override;
    void wheelEvent( QWheelEvent *event ) override;
    void keyPressEvent( QKeyEvent *event ) override;
    void keyReleaseEvent( QKeyEvent *event ) override;
    void focusOutEvent( QFocusEvent *event ) override;
};

#endif // GRAPHICS_VIEW_HPP