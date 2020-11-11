#include <curve_view.hpp>

curve_view::curve_view(/* args */)
{
    this->setMouseTracking(true);
    create_curve();

    qt::shortcut("+", this, [this]() {
        if (curves.contains(drag_curve))
        {
            key_frame &key = curves[drag_curve][drag_index];
            key.exaggeration += 0.2;
            update();
        }
    });

    qt::shortcut("-", this, [this]() {
        if (curves.contains(drag_curve))
        {
            key_frame &key = curves[drag_curve][drag_index];
            key.exaggeration -= 0.2;
            update();
        }
    });
}

curve_view::~curve_view()
{
}

QLineF curve_view::get_handler_points(
    key_frame key,
    key_frame previous_key,
    key_frame next_key,
    bool infinite)
{
    float separation_a = (key.pos.x() - previous_key.pos.x()) * key.exaggeration;
    float separation_b = (next_key.pos.x() - key.pos.x()) * key.exaggeration;

    // si es el primer o ultimo keyframe, le asigna la separacion del lado contrario
    if (!previous_key.init)
        separation_a = separation_b;
    if (!next_key.init)
        separation_b = separation_a;
    //
    //

    float point_a_x = key.pos.x() - separation_a;
    float point_b_x = key.pos.x() + separation_b;

    QPointF point_a = {point_a_x, key.pos.y()};
    QPointF point_b = {point_b_x, key.pos.y()};

    // genera el punto vertical infinito donde apunta el manejador
    float tangent = tan(key.angle * M_PI / 180);
    QPointF infinite_point_a = {point_a.x(), point_a.y() + (tangent * separation_a)};
    QPointF infinite_point_b = {point_b.x(), point_b.y() - (tangent * separation_b)};
    //

    if (!infinite)
    {
        // antes de rotar el manejador, transforma el punto de key y los
        // 2 puntos del manejador de cordenadas a puntos en la
        // position del visor, con esto logramos que el manejador
        // siempre quede del mismo tamaño.
        QPointF view_point_a = get_position(point_a);
        QPointF view_point_b = get_position(point_b);

        QPointF view_infinite = get_position(infinite_point_a);
        QPointF view_anchor_point = get_position(key.pos);

        float angle = get_angle_two_points(view_infinite, view_anchor_point) - 90;

        view_point_a = rotate_point(view_point_a, view_anchor_point, angle - 180);
        view_point_b = rotate_point(view_point_b, view_anchor_point, angle);

        point_a = get_coords({view_point_a.x(), view_point_a.y()});
        point_b = get_coords({view_point_b.x(), view_point_b.y()});
    }

    if (infinite)
        return {infinite_point_a, infinite_point_b};
    else
        return {point_a, point_b};
}

QPointF curve_view::cubic_bezier(
    QPointF p1, QPointF p2,
    QPointF p3, QPointF p4,
    float value)
{
    // Algoritmo bezier
    QPointF L1 = ((1 - value) * p1) + (value * p2);
    QPointF L2 = ((1 - value) * p2) + (value * p3);
    QPointF L3 = ((1 - value) * p3) + (value * p4);

    QPointF Q1 = ((1 - value) * L1) + (L2 * value);
    QPointF Q2 = ((1 - value) * L2) + (L3 * value);

    return ((1 - value) * Q1) + (Q2 * value);
    //
}

void curve_view::create_curve()
{
    key_frame key1 = {{0.1, 0.2}, 10, false, true, 1, 0};
    key_frame key2 = {{0.5, 1}, 45, false, true, 0.3, 0};
    key_frame key3 = {{1, 0.3}, 0, false, true, 0.5, 0};
    key_frame key4 = {{2, 0.3}, 0, false, true, 0.5, 0};

    curves.insert("translate_x", {key1, key2, key3, key4});

    update();
}

void curve_view::key_press(QPoint cursor_position)
{
    // si el click del mouse fue presionado en algun keyframe o en
    // alguno de los 2 puntos del manejador, los asigna a las variables de 'drag'

    int draw_tolerance = 10; // Pixels
    for (auto curve_name : curves.keys())
    {
        auto &curve = curves[curve_name];
        for (int i = 0; i < curve.count(); i++)
        {
            key_frame previous_key = curve.value(i - 1);
            key_frame next_key = curve.value(i + 1);

            key_frame &key = curve[i];

            QLineF handler = get_handler_points(key, previous_key, next_key);

            int handler_point = 0;

            // verifica si el click se hizo en alguno de los 2 puntos manejadores
            float distance = qt::distance_points(get_position(handler.p1()), cursor_position);
            if (distance < draw_tolerance)
                handler_point = 1;

            distance = qt::distance_points(get_position(handler.p2()), cursor_position);
            if (distance < draw_tolerance)
                handler_point = 2;
            //
            //

            // verifica si el click se hizo en el key point
            if (!handler_point)
            {
                QPointF key_position = get_position(key.pos);
                float distance = qt::distance_points(key_position, cursor_position);
                if (distance < draw_tolerance)
                    key.selected = true;

                else
                    key.selected = false;
            }
            //
            //

            if (key.selected)
            {
                drag_index = i;
                drag_curve = curve_name;
                drag_handler = handler_point;
                is_drag = true;
            }
        }
    }
}

void curve_view::key_move(QPoint cursor_position)
{
    if (is_drag)
    {
        if (curves.contains(drag_curve))
        {
            key_frame &key = curves[drag_curve][drag_index];

            if (drag_handler == 1)
                key.angle = 90 - get_angle_two_points(get_coords(cursor_position), key.pos);

            else if (drag_handler == 2)
                key.angle = 270 - get_angle_two_points(get_coords(cursor_position), key.pos);
            else
                key.pos = get_coords(cursor_position);
        }
    }
}

bool curve_view::is_point_in_rectangle(QPointF point, QLineF rectangle)
{
    // verifica si un punto esta dentro de un rectangulo,
    // el rectangulo esta representado por una linea en diagonal.
    float x = point.x();
    float y = point.y();

    QPointF p1 = rectangle.p1();
    QPointF p2 = rectangle.p2();

    bool iside_x = false;
    bool iside_y = false;

    if (x > p1.x() and x < p2.x() || x < p1.x() and x > p2.x())
        iside_x = true;
    if (y > p1.y() and y < p2.y() || y < p1.y() and y > p2.y())
        iside_y = true;

    return (iside_x && iside_y);
}

void curve_view::selector_move(QPoint cursor_position)
{
    selector.setP2(get_coords(cursor_position));

    for (auto &curve : curves)
    {
        for (key_frame &key : curve)
        {
            if (is_point_in_rectangle(key.pos, selector))
                key.selected = true;
            else
                key.selected = false;
        }
    }
}

void curve_view::mousePressEvent(QMouseEvent *event)
{
    if (!qt::alt())
    {
        key_press(event->pos());

        show_selector = true;
        selector.setP1(get_coords(event->pos()));
    }

    update();
    gl_view::mousePressEvent(event);
}

void curve_view::mouseReleaseEvent(QMouseEvent *event)
{
    is_drag = false;
    show_selector = false;
    gl_view::mouseReleaseEvent(event);
}

void curve_view::mouseMoveEvent(QMouseEvent *event)
{
    if (!qt::alt())
    {
        key_move(event->pos());
        selector_move(event->pos());
    }

    update();
    gl_view::mouseMoveEvent(event);
}