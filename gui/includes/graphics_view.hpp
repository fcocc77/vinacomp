#ifndef GRAPHICS_VIEW_HPP
#define GRAPHICS_VIEW_HPP

#include <QGraphicsView>
#include <QScrollBar>
#include <util.hpp>

class graphics_view : public QGraphicsView
{
    Q_OBJECT
private:
    bool panning;
    bool zooming;
    bool pressed;
    int panning_start_x, panning_start_y;

public:
    graphics_view(/* args */);
    ~graphics_view();

protected:
    // Events
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // GRAPHICS_VIEW_HPP