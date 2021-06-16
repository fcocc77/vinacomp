#include <curve_view.h>
#include <qt.h>
#include <vinacomp.h>

curve_view::curve_view(QWidget *__vinacomp)
    : _vinacomp(__vinacomp)
    , drag_key_frame(nullptr)
    , dragging(false)
    , text_visible(false)
    , dragging_cursor(false)
    , player_cursor_visible(true)
    , transform_box_visible(false)
    , unselected_keys(false)
    , selecting(false)
    , transforming(false)
{
    project = static_cast<vinacomp *>(_vinacomp)->get_project();

    popup_setup_ui();

    qt::shortcut("+", this, [this]() {
        if (!underMouse())
            return;

        for (key_frame *key : get_selected_keys())
            key->set_exaggeration(key->exaggeration() + 0.2);

        update();
    });

    qt::shortcut("-", this, [this]() {
        if (!underMouse())
            return;

        for (key_frame *key : get_selected_keys())
            key->set_exaggeration(key->exaggeration() - 0.2);

        update();
    });

    fit_viewport_to_frame_range();
}

curve_view::~curve_view() {}

void curve_view::select_all_key_frames()
{
    for (curve *_curve : curves)
        for (key_frame *key : _curve->get_keys())
            key->select(true);

    show_transform_box();
    text_visible = false;
}

void curve_view::fit_viewport_to_keyframes()
{
    int padding_percent = 20;

    QList<key_frame *> all_key_frames;
    for (curve *_curve : curves)
        for (key_frame *key : _curve->get_keys())
            all_key_frames.push_back(key);

    auto selected = get_selected_keys();

    QLineF rect;
    if (selected.count() > 1)
        rect = get_rectangle_of_keyframes(selected);
    else if (all_key_frames.count() > 1)
        rect = get_rectangle_of_keyframes(all_key_frames);
    else if (!all_key_frames.empty())
    {
        key_frame *alone_key = all_key_frames[0];
        fit_viewport_to_frame_range({alone_key->x(), alone_key->y()});
        return;
    }
    else
    {
        fit_viewport_to_frame_range();
        return;
    }

    float x_distance = (rect.x2() - rect.x1()) * get_aspect();
    float y_distance = rect.y2() - rect.y1();

    if (y_distance == 0)
        y_distance = 1;

    float padding_x = (padding_percent * x_distance) / 100.0;
    float padding_y = (padding_percent * y_distance) / 100.0;

    set_ortho(rect.x1() - padding_x, rect.x2() + padding_x, rect.y1() - padding_y,
              rect.y2() + padding_y);
}

void curve_view::fit_viewport_to_frame_range(QPointF offset)
{
    int padding_percent = 20;

    project_settings *settings =
        static_cast<vinacomp *>(_vinacomp)->get_project_settings();

    float x1 = settings->get_first_frame() + offset.x();
    float x2 = settings->get_last_frame() + offset.x();

    float y1 = offset.y();
    float y2 = offset.y() + 1;

    float x_distance = x2 - x1;
    float y_distance = y2 - y1;

    QLineF rect;
    rect.setP1({x1, y1});
    rect.setP2({x2, y2});

    float padding_x = (padding_percent * x_distance) / 300.0;
    float padding_y = (padding_percent * y_distance) / 100.0;

    set_ortho(rect.x1() - padding_x, rect.x2() + padding_x,
              rect.y1() - padding_y, rect.y2() + padding_y);
}

void curve_view::mousePressEvent(QMouseEvent *event)
{
    click_position = event->pos();

    if (!qt::alt() && event->button() == Qt::LeftButton)
    {
        text_visible = false;

        transform_box_press(event->pos());
        cursor_press(event->pos());

        if (!transforming)
        {
            transform_box_visible = false;
            key_press(event->pos());

            if (!dragging && !dragging_cursor)
            {
                selecting = true;
                QPointF coords = get_coords(event->pos());
                selector.setP1(coords);
                selector.setP2(coords);
            }
        }
    }

    popup_press(event);

    update();
    gl_view::mousePressEvent(event);
}

void curve_view::mouseReleaseEvent(QMouseEvent *event)
{
    if (!transforming && drag_key_frame)
        change_curve(curves.value(drag_key_frame->get_curve()));

    if (selecting)
        show_transform_box();

    text_visible = get_selected_keys().count() <= 1;

    dragging = false;
    selecting = false;
    transforming = false;
    drag_key_frame = nullptr;
    dragging_cursor = false;

    this->setCursor(Qt::ArrowCursor);

    update();

    gl_view::mouseReleaseEvent(event);
}

void curve_view::cursor_move_event(QPoint position)
{
    this->setCursor(Qt::ArrowCursor);

    if (!qt::alt())
    {
        transform_box_move(position);
        cursor_move(position);
        if (!transforming)
        {
            key_move(position);
            selector_move(position);
        }
    }

    gl_view::cursor_move_event(position);
}

void curve_view::clear()
{
    for (curve *_curve : curves)
        delete _curve;

    curves.clear();
    transform_box_visible = false;

    update();
}
