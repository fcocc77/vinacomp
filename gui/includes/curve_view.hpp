#ifndef CURVE_VIEW_HPP
#define CURVE_VIEW_HPP

#include <gl_view.hpp>

class curve_view : public gl_view
{

private:
    struct key_frame
    {
        QPointF pos;
        int angle;
        float exaggeration; // 0.0 - 1.0
        int interpolation;
        // 0: Linear
        // 1: Horizontal
        // 2: Smooth
        // 3: Break
        // 4: Custom
    };

    QMap<QString, QList<key_frame>> curves;

    // Drag KeyFrame
    QString drag_curve;
    int drag_index;
    int drag_handler;
    bool is_drag = false;
    //

    void draw_circle();
    void draw_grid();
    void draw_coordinate_numbers();
    void draw_point(QPointF coords);
    void create_curve();
    void draw_curve();
    void key_press(QPoint cursor_position);
    QLineF get_handler_points(key_frame key);
    QPointF rotate_point(QPointF point, QPointF anchor_point, float angle, bool keep_aspect = false);
    float get_angle_two_points(QPointF a, QPointF b);
    float get_angle_orientation(float angle);
    void draw_bezier(QPointF src_key, QPointF dst_key);
    QPointF cubic_bezier(
        QPointF point_a,
        QPointF point_b,
        float value,
        float exaggeration = 0.34);

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