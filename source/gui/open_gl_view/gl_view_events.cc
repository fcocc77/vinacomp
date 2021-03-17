#include <gl_view.h>

void gl_view::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        scale = scale / 1.1;
    else
        scale = scale * 1.1;

    update();
}

void gl_view::mousePressEvent(QMouseEvent *event)
{
    click_position = event->pos();
    center_button = event->button() == Qt::MidButton;
    pressed = true;

    box_handler_press(click_position);
    pos_handler_press(click_position);
    tf_handler_press(click_position);

    this->setFocus();

    if (event->button() == Qt::MidButton)
    {
        if (qt::alt())
        {
            zooming = true;
            click_scale = scale;

            // se resta la posicion del click, a la cordenada,
            // y despues se suma en el 'mouseMoveEvent', y asi se logra el pundo de
            // anclaje donde esta el cursor.
            click_translate = translate - get_coordinate(click_position);
            //
        }
        else
        {
            click_translate = translate;
            panning = true;
        }
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (qt::alt())
        {
            click_translate = translate;
            panning = true;
        }
    }
}

void gl_view::mouseReleaseEvent(QMouseEvent *event)
{
    panning = false;
    zooming = false;
    pressed = false;

    box_handler_release(event->pos());
    pos_handler_release(event->pos());
    tf_handler_release(event->pos());

    update();
}

void gl_view::cursor_move_event(QPoint position)
{
    qt::focus_under_mouse(this);

    if (panning)
    {
        // le resta la cordenada del click para que el paneo comience en 0
        QPointF coord_to_add = get_coordinate(position) - get_coordinate(click_position);
        //
        translate = click_translate + coord_to_add;
        update();
    }
    if (zooming)
    {
        float scale_speed = 1.007;

        float scale_x_to_add = click_position.x() - position.x();
        float scale_y_to_add = position.y() - click_position.y();

        double scale_factor_x = pow(scale_speed, scale_x_to_add);
        double scale_factor_y = pow(scale_speed, scale_y_to_add);

        float scale_x = click_scale.x() * scale_factor_x;
        float scale_y = click_scale.y() * scale_factor_y;

        if (lock_scale)
            scale_y = scale_x;

        float min = 0.02;
        float max = 100000;

        // limitar zoom Horizontal
        if (scale_x < min)
            scale_x = min;
        else if (scale_x > max)
            scale_x = max;
        //

        // limitar zoom Vertical
        if (scale_y < min)
            scale_y = min;
        else if (scale_y > max)
            scale_y = max;
        //

        scale = {scale_x, scale_y};

        // punto de anclaje
        translate = click_translate + get_coordinate(click_position);
        //

        update();
    }
};

void gl_view::mouseMoveEvent(QMouseEvent *event)
{
    if (!qt::alt() && !center_button)
    {
        box_handler_move(event->pos());
        pos_handler_move(event->pos());
        tf_handler_move(event->pos());
    }

    cursor_move_event(event->pos());
    QOpenGLWidget::mouseMoveEvent(event);
}

void gl_view::tabletEvent(QTabletEvent *event)
{
    if (!pressed)
        cursor_move_event(event->pos());

    QOpenGLWidget::tabletEvent(event);
}

void gl_view::showEvent(QShowEvent *event)
{
    update();
}
