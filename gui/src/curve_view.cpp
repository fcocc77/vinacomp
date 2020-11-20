#include <curve_view.hpp>

curve_view::curve_view()
{
    this->setMouseTracking(true);
    this->setTabletTracking(true);

    create_curve("Translate", Qt::cyan, 0);
    create_curve("Scale", Qt::green, 1);
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
}

curve_view::~curve_view()
{
}

void curve_view::select_all_key_frames()
{
    for (auto keys : curves)
        for (key_frame *key : keys)
            key->select(true);

    show_transform_box();
}

void curve_view::fit_viewport_to_keyframes()
{
    int padding_percent = 20;

    QList<key_frame *> all_key_frames;
    for (auto keys : curves)
        for (key_frame *key : keys)
            all_key_frames.push_back(key);

    auto selected = get_selected_keys();

    QLineF rect;

    if (selected.count() > 1)
        rect = get_rectangle_of_keyframes(selected);
    else if (!all_key_frames.empty())
        rect = get_rectangle_of_keyframes(all_key_frames);
    else
    {
        set_default();
        return;
    }

    float x_distance = (rect.x2() - rect.x1()) * get_aspect();
    float y_distance = rect.y2() - rect.y1();

    float padding_x = (padding_percent * x_distance) / 100.0;
    float padding_y = (padding_percent * y_distance) / 100.0;

    set_ortho(
        rect.x1() - padding_x,
        rect.x2() + padding_x,
        rect.y1() - padding_y,
        rect.y2() + padding_y);
}

void curve_view::mousePressEvent(QMouseEvent *event)
{
    click_position = event->pos();

    if (!qt::alt() && event->button() == Qt::LeftButton)
    {
        transform_box_press(event->pos());
        if (!transforming)
        {
            transform_box_visible = false;
            key_press(event->pos());

            if (!dragging)
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
    if (selecting)
        show_transform_box();

    dragging = false;
    selecting = false;
    transforming = false;

    update();

    gl_view::mouseReleaseEvent(event);
}

void curve_view::move_event(QPoint position)
{
    this->setCursor(Qt::ArrowCursor);

    if (!qt::alt())
    {
        transform_box_move(position);
        if (!transforming)
        {
            key_move(position);
            selector_move(position);
        }
    }
}

void curve_view::mouseMoveEvent(QMouseEvent *event)
{
    move_event(event->pos());
    gl_view::mouseMoveEvent(event);
}

void curve_view::tabletEvent(QTabletEvent *event)
{
    move_event(event->pos());
    gl_view::tabletEvent(event);
}
