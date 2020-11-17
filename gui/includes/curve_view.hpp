#ifndef CURVE_VIEW_HPP
#define CURVE_VIEW_HPP

#include <gl_view.hpp>
#include <curve_key_frame.hpp>

class curve_view : public gl_view
{

private:
    QPoint click_position;

    // Drag KeyFrame
    int drag_handler;
    key_frame *drag_key_frame;
    bool dragging = false;
    //
    //

    // Key Frames
    QMap<QString, QList<key_frame *>> curves;

    bool is_point_in_rectangle(QPointF point, QLineF rectangle);
    void fit_viewport_to_keyframes();
    void select_all_key_frames();
    void create_curve();
    void key_press(QPoint cursor_position);
    void key_move(QPoint cursor_position);
    key_frame *get_previous_key(key_frame *key);
    key_frame *get_next_key(key_frame *key);

    key_frame empty_key;
    QLineF get_handler_points(key_frame *key, bool infinite = false);
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
    void draw_bezier(key_frame *src_key, key_frame *dst_key);
    void draw_selector();
    void draw_transform_box();
    //
    //
    //

    // Selector
    bool transform_box_visible = false;
    QLineF transform_box;
    QLineF last_transform_box;

    bool selecting = false;
    QLineF selector;

    QLineF max_translate_left;
    QLineF max_translate_right;

    bool transforming = false;
    QString resize_current_action;
    QList<key_frame *> selected_key_frames;

    void selector_move(QPoint cursor_position);
    QList<key_frame *> get_selected_keys();
    QString get_transform_action(QPoint cursor_position);
    QLineF get_rectangle_of_keyframes(QList<key_frame *> keys);
    void scale_key_from_point(QPointF point);
    void translate_keys(QPointF add_translate);
    void to_transform_box(QPoint cursor_position);
    void transform_box_press(QPoint cursor_position);
    void transform_box_move(QPoint cursor_position);
    void show_transform_box();
    QLineF get_max_translate(QPoint cursor_position, QString orientation);
    bool key_overlap(key_frame *key);

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