#ifndef CURVE_VIEW_HPP
#define CURVE_VIEW_HPP

#include <gl_view.hpp>

struct key_frame
{
    QPointF pos;
    float angle;
    bool selected;
    bool init;          // Inicializacion
    float exaggeration; // 0.0 - 1.0
    int interpolation;
    QString curve;
    int index;
    // 0: Linear
    // 1: Horizontal
    // 2: Smooth
    // 3: Break
    // 4: Custom
};

class curve_view : public gl_view
{

private:
    QMap<QString, QList<key_frame>> curves;

    QPoint click_position;

    // Drag KeyFrame
    QString drag_curve;
    int drag_index;
    int drag_handler;
    bool dragging = false;
    //
    //

    //
    bool is_point_in_rectangle(QPointF point, QLineF rectangle);
    void create_curve();
    void key_press(QPoint cursor_position);
    void key_move(QPoint cursor_position);

    QLineF get_handler_points(
        key_frame key,
        key_frame previous_key = {},
        key_frame next_key = {},
        bool infinite = false);
    QPointF cubic_bezier(
        QPointF p1, QPointF p2,
        QPointF p3, QPointF p4,
        float value);
    //
    //

    // Paint
    void draw_circle();
    void draw_grid();
    void draw_coordinate_numbers();
    void draw_curve();
    void draw_bezier(key_frame src_key, key_frame dst_key);
    void draw_selector();
    void draw_resize_box();
    //
    //
    //

    // Selector
    bool resize_box_visible = false;
    QLineF resize_box;
    QLineF last_resize_box;

    bool selecting = false;
    QLineF selector;

    bool transforming = false;
    QString resize_current_action;
    QList<key_frame> key_frame_press;

    void selector_move(QPoint cursor_position);
    QList<key_frame> get_selected_keys();
    QString get_resize_action(QPoint cursor_position);
    QLineF get_rectangle_of_selected_keyframes();
    void scale_key_from_point(QPointF point);
    void translate_keys(QPointF add_translate);


    void resize_box_press(QPoint cursor_position);
    void resize_box_move(QPoint cursor_position);
    //
    //

public:
    curve_view(/* args */);
    ~curve_view();

protected:
    void initializeGL() override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void tabletEvent(QTabletEvent *event);
    void move_event(QPoint position);
};

#endif // CURVE_VIEW_HPP