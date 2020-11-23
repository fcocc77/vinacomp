#ifndef CURVE_VIEW_HPP
#define CURVE_VIEW_HPP

#include <QMenu>

#include <gl_view.h>
#include <action.h>
#include <curve_key_frame.h>
#include <vinacomp_namespace.h>

class curve_view : public gl_view
{

private:
    QPoint click_position;

    // Drag KeyFrame
    int drag_handler;
    key_frame *drag_key_frame;
    bool dragging;
    //
    //

    // Key Frames
    QMap<QString, QList<key_frame *>> curves;
    bool text_visible;

    bool is_point_in_rectangle(QPointF point, QLineF rectangle);
    void fit_viewport_to_keyframes();
    void select_all_key_frames();
    void set_interpolation_to_selected(int number);
    void key_press(QPoint cursor_position);
    void key_move(QPoint cursor_position);
    key_frame *get_previous_key(key_frame *key);
    key_frame *get_next_key(key_frame *key);
    void orient_linear_handler(key_frame *key);

    QLineF get_handler_points(key_frame *key, bool infinite = false);
    QPointF cubic_bezier(
        QPointF p1, QPointF p2,
        QPointF p3, QPointF p4,
        float value);
    //
    //

    // Popup
    QMenu *popup;
    void popup_setup_ui();
    void popup_press(QMouseEvent *event);
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
    bool transform_box_visible;
    QLineF transform_box;
    QLineF last_transform_box;

    bool unselected_keys;
    bool selecting;
    QLineF selector;

    QLineF limit_coord_left;
    QLineF limit_coord_right;
    QLineF max_translate;

    bool transforming;
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
    QLineF get_limit_coord(QString orientation);
    QLineF get_max_translate(QPoint cursor_position);

    bool key_overlap(key_frame *key);

    //
    //

public:
    curve_view(/* args */);
    ~curve_view();

    // Acciones
    action *select_all_action;
    action *delete_keyframes_action;
    action *linear_action;
    action *horizontal_action;
    action *smooth_action;
    action *break_action;
    action *center_action;
    //
    //

    //Key Frames
    void create_curve(QString name, QColor color, QJsonArray keys);
    //

protected:
    void initializeGL() override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void tabletEvent(QTabletEvent *event) override;
    void move_event(QPoint position);
};

#endif // CURVE_VIEW_HPP