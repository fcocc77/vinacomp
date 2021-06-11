#ifndef CURVE_VIEW_HPP
#define CURVE_VIEW_HPP

#include <QMenu>

#include <action.h>
#include <animation.h>
#include <curve_key_frame.h>
#include <curve_curve.h>
#include <gl_view.h>
#include <project_struct.h>

class curve_view : public gl_view
{
    Q_OBJECT
private:
    QPoint click_position;
    project_struct *project;

    // Drag KeyFrame
    int drag_handler;
    key_frame *drag_key_frame;
    bool dragging;
    //
    //

    // Key Frames
    QMap<QString, curve *> curves;
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
    void delete_selected_keyframes();

    QLineF get_handler_points(key_frame *key, bool infinite = false);
    //

    // Popup
    QMenu *popup;
    void popup_setup_ui();
    void popup_press(QMouseEvent *event);
    //
    //

    // Paint
    void draw_grid();
    void draw_coordinate_numbers();
    void draw_curve();
    void draw_bezier(key_frame *src_key, key_frame *dst_key);
    void draw_selector();
    void draw_transform_box();
    void draw_cursor();
    //
    //
    //

    // Cursor
    bool dragging_cursor;
    bool player_cursor_visible;
    bool is_over_cursor(QPoint cursor_position);
    void cursor_press(QPoint cursor_position);
    void cursor_move(QPoint cursor_position);
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
    curve_view(project_struct *project);
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

    // Key Frames
    void create_curve(QString name, QColor color, QList<anim::key_data> keys);
    void delete_curve(QString node_name);
    void clear();
    void set_cursor_visibility(bool visible);
    //

protected:
    void initializeGL() override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void cursor_move_event(QPoint position) override;

signals:
    void change_curve(curve *_curve);
    void delete_keyframes(QMap<QString, QList<int>> curves);
    void change_frame(int frame);
};

#endif // CURVE_VIEW_HPP
