#ifndef CURVE_VIEW_HPP
#define CURVE_VIEW_HPP

#include <QHoverEvent>
#include <gl_view.hpp>

class curve_view : public gl_view
{

private:
    QMap<QString, QList<QPointF>> curves;

    // Drag KeyFrame
    QString drag_curve;
    int drag_index;
    bool is_drag;
    //

    void draw_circle();
    void draw_grid();
    void draw_coordinate_numbers();
    void create_handle(QPoint position);
    void draw_point(QPointF coords);
    void create_curve();
    void draw_curve();

public:
    curve_view(/* args */);
    ~curve_view();

protected:
    void initializeGL() override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // CURVE_VIEW_HPP