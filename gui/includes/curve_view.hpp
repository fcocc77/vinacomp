#ifndef CURVE_VIEW_HPP
#define CURVE_VIEW_HPP

#include <gl_view.hpp>

class curve_view : public gl_view
{

private:
    QPointF point;

    void draw_circle();
    void draw_grid();
    void draw_coordinate_numbers();

public:
    curve_view(/* args */);
    ~curve_view();

protected:
    void initializeGL() override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event);
};

#endif // CURVE_VIEW_HPP