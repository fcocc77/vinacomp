#include <curve_view.hpp>

curve_view::curve_view(/* args */)
{
    this->setMouseTracking(true);
    this->setTabletTracking(true);

    create_curve();

    qt::shortcut("+", this, [this]() {
        for (key_frame key : get_selected_keys())
            curves[key.curve][key.index].exaggeration += 0.2;

        update();
    });

    qt::shortcut("-", this, [this]() {
        for (key_frame key : get_selected_keys())
            curves[key.curve][key.index].exaggeration -= 0.2;

        update();
    });
}

curve_view::~curve_view()
{
}

void curve_view::mousePressEvent(QMouseEvent *event)
{
    click_position = event->pos();

    if (!qt::alt() && event->button() == Qt::LeftButton)
    {
        resize_box_press(event->pos());
        if (!transforming)
        {
            resize_box_visible = false;
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

    update();
    gl_view::mousePressEvent(event);
}

void curve_view::mouseReleaseEvent(QMouseEvent *event)
{
    if (selecting)
    {
        resize_box = get_rectangle_of_selected_keyframes();
        resize_box_visible = !resize_box.isNull();
    }

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
        resize_box_move(position);
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
