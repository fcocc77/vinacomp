#include <curve_view.hpp>

QLineF curve_view::get_handler_points(
    key_frame key,
    key_frame previous_key,
    key_frame next_key,
    bool infinite)
{
    float separation_a = (key.x() - previous_key.x()) * key.exaggeration();
    float separation_b = (next_key.x() - key.x()) * key.exaggeration();

    // si es el primer o ultimo keyframe, le asigna la separacion del lado contrario
    if (previous_key.is_null())
        separation_a = separation_b;
    if (next_key.is_null())
        separation_b = separation_a;
    //
    //

    float point_a_x = key.x() - separation_a;
    float point_b_x = key.x() + separation_b;

    QPointF point_a = {point_a_x, key.y()};
    QPointF point_b = {point_b_x, key.y()};

    // genera el punto vertical infinito donde apunta el manejador
    float tangent = tan(key.get_angle() * M_PI / 180);
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
        QPointF view_anchor_point = get_position(key.pos());

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
    QString name = "translate_x";

    key_frame key1(name, 0);
    key_frame key2(name, 1);
    key_frame key3(name, 2);
    key_frame key4(name, 3);

    key1.set_pos({0.1, 0.2});
    key2.set_pos({0.5, 1});
    key3.set_pos({1, 0.3});
    key4.set_pos({2, 0.4});

    curves.insert(name, {key1, key2, key3, key4});

    update();
}

void curve_view::key_press(QPoint cursor_position)
{
    // si el click del mouse fue presionado en algun keyframe o en
    // alguno de los 2 puntos del manejador, los asigna a las variables de 'drag'

    for (auto curve_name : curves.keys())
    {
        auto &curve = curves[curve_name];
        for (int i = 0; i < curve.count(); i++)
        {
            key_frame previous_key = curve.value(i - 1);
            key_frame next_key = curve.value(i + 1);

            key_frame &key = curve[i];

            QLineF handler = get_handler_points(key, previous_key, next_key);

            // verifica si el click se hizo en el key point
            // o se hizo en alguno de los 2 puntos manejadores.
            int handler_point = 0;

            if (is_cursor_above(cursor_position, key.pos()))
                key.select(true);
            else if (is_cursor_above(cursor_position, handler.p1()))
                handler_point = 1;
            else if (is_cursor_above(cursor_position, handler.p2()))
                handler_point = 2;
            else
                key.select(false);
            //
            //

            if (key.selected())
            {
                drag_index = i;
                drag_curve = curve_name;
                drag_handler = handler_point;
                dragging = true;
            }
        }
    }
}

void curve_view::key_move(QPoint cursor_position)
{

    // cambia el cursor si esta sobre algun key frame
    if (!resize_box_visible)
        for (auto keys : curves)
            for (key_frame key : keys)
                if (is_cursor_above(cursor_position, key.pos()))
                    this->setCursor(Qt::SizeAllCursor);
    //
    //

    if (dragging)
    {
        if (curves.contains(drag_curve))
        {
            auto &keys = curves[drag_curve];
            key_frame &key = keys[drag_index];

            if (drag_handler == 1)
                key.set_angle(90 - get_angle_two_points(get_coords(cursor_position), key.pos()));

            else if (drag_handler == 2)
                key.set_angle(270 - get_angle_two_points(get_coords(cursor_position), key.pos()));
            else
            {
                QPointF coords = get_coords(cursor_position);

                // limitar posicion al key_frame anterior y siguiente
                key_frame previous_key = keys.value(key.get_index() - 1);
                if (!previous_key.is_null())
                {
                    if (coords.x() < previous_key.x())
                        coords.setX(previous_key.x());
                }

                key_frame next_key = keys.value(key.get_index() + 1);
                if (!next_key.is_null())
                {
                    if (coords.x() > next_key.x())
                        coords.setX(next_key.x());
                }
                //
                //

                key.set_pos(coords);
            }
        }

        update();
    }
}
