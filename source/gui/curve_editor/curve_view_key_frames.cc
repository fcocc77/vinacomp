#include <curve_utils.h>
#include <curve_view.h>
#include <vinacomp_namespace.h>
#include <util.h>

QLineF curve_view::get_handler_points(key_frame *key, bool infinite)
{
    key_frame *previous_key = get_previous_key(key);
    key_frame *next_key = get_next_key(key);

    float left_separation, right_separation;

    if (previous_key)
        left_separation = (key->x() - previous_key->x()) * key->exaggeration();
    if (next_key)
        right_separation = (next_key->x() - key->x()) * key->exaggeration();

    // si es el primer o ultimo keyframe, le asigna la separacion del lado
    // contrario
    if (!previous_key)
        left_separation = right_separation;
    if (!next_key)
        right_separation = left_separation;
    //
    //

    // genera el punto vertical infinito donde apunta el manejador
    QLineF infinite_handlers = handler_points(
        key->pos(), key->get_left_angle(), key->get_right_angle(),
        left_separation, right_separation);

    QPointF infinite_left_point = infinite_handlers.p1();
    QPointF infinite_right_point = infinite_handlers.p2();
    //

    if (infinite)
    {
        return {infinite_left_point, infinite_right_point};
    }
    else
    {
        // antes de rotar el manejador, transforma el punto de key y los
        // 2 puntos del manejador de cordenadas a puntos en la
        // position del visor, con esto logramos que el manejador
        // siempre quede del mismo tamaño.
        float left_point_x = key->x() - left_separation;
        float right_point_x = key->x() + right_separation;

        QPointF left_point = {left_point_x, key->y()};
        QPointF right_point = {right_point_x, key->y()};

        QPointF viwport_left_point = get_position(left_point);
        QPointF viwport_right_point = get_position(right_point);

        QPointF view_infinite_left = get_position(infinite_left_point);
        QPointF view_infinite_right = get_position(infinite_right_point);

        QPointF view_anchor_point = get_position(key->pos());

        float left_angle =
            get_angle_two_points(view_infinite_left, view_anchor_point) - 90;
        float right_angle =
            get_angle_two_points(view_infinite_right, view_anchor_point) + 90;

        viwport_left_point = rotate_point(viwport_left_point, view_anchor_point,
                                          left_angle - 180);
        viwport_right_point =
            rotate_point(viwport_right_point, view_anchor_point, right_angle);

        left_point = get_coordsf(viwport_left_point);
        right_point = get_coordsf(viwport_right_point);

        return {left_point, right_point};
    }
}

void curve_view::create_curve(QString name, QColor color,
                              QList<anim::key_data> keys)
{
    if (keys.empty())
        return;

    if (curves.contains(name))
        return;

    curve *_curve = new curve(name);

    for (int i = 0; i < keys.count(); i++)
    {
        auto kdata = keys[i];
        QPointF position = {(float)kdata.frame, kdata.value};

        key_frame *_key = _curve->add_key(i, position, color);

        _key->set_left_angle(kdata.left_angle);
        _key->set_right_angle(kdata.right_angle);
        _key->set_interpolation(vina::custom, vina::custom);
    }

    curves.insert(name, _curve);

    update();
}

void curve_view::delete_curve(QString name)
{
    if (!curves.contains(name))
        return;

    delete curves.value(name);
    curves.remove(name);

    update();
}

key_frame *curve_view::get_previous_key(key_frame *key)
{
    QString curve_name = key->get_curve();
    if (!curves.contains(curve_name))
        return nullptr;

    return curves.value(curve_name)->get_previous_key(key);
}

key_frame *curve_view::get_next_key(key_frame *key)
{
    QString curve_name = key->get_curve();
    if (!curves.contains(curve_name))
        return nullptr;

    return curves.value(curve_name)->get_next_key(key);
}

void curve_view::set_interpolation_to_selected(int number)
{
    for (key_frame *key : get_selected_keys())
    {
        key->set_interpolation(number, number);
        if (number == 0)
            orient_linear_handler(key);
    }

    update();
}

void curve_view::orient_linear_handler(key_frame *key)
{
    // si el handler es lineal, orienta el 'handler' a la direccion
    // del 'handler' del siguiente y anterior keyframe.

    if (key->left_interpolation() == 0)
    {
        key_frame *previous_key = get_previous_key(key);

        if (previous_key)
        {
            QLineF previous_handler = get_handler_points(previous_key);

            float angle =
                90 - get_angle_two_points(key->pos(), previous_handler.p2());
            key->set_left_angle(angle);
        }
    }

    if (key->right_interpolation() == 0)
    {
        key_frame *next_key = get_next_key(key);

        if (next_key)
        {
            QLineF next_handler = get_handler_points(next_key);

            float angle =
                270 - get_angle_two_points(key->pos(), next_handler.p1());
            key->set_right_angle(-angle);
        }
    }
}

void curve_view::key_press(QPoint cursor_position)
{
    // si el click del mouse fue presionado en algun keyframe o en
    // alguno de los 2 puntos del manejador, los asigna a las variables de
    // 'drag'

    for (QString curve_name : curves.keys())
    {
        curve *_curve = curves[curve_name];
        for (key_frame *key : _curve->get_keys())
        {
            QLineF handler = get_handler_points(key);

            // verifica si el click se hizo en el key point
            // o se hizo en alguno de los 2 puntos manejadores.
            int handler_point = 0;

            if (cursor_above_point(cursor_position, key->pos()))
                key->select(true);
            else if (cursor_above_point(cursor_position, handler.p1()))
            {
                handler_point = 1;
                key->set_interpolation(vina::custom, vina::none);
            }
            else if (cursor_above_point(cursor_position, handler.p2()))
            {
                handler_point = 2;
                key->set_interpolation(vina::none, vina::custom);
            }
            else
                key->select(false);
            //
            //

            if (key->selected())
            {
                key->stamp_position();
                drag_key_frame = key;
                drag_handler = handler_point;
                dragging = true;
                text_visible = true;
            }
        }
    }
}

void curve_view::key_move(QPoint cursor_position)
{

    // cambia el cursor si esta sobre algun key frame
    if (!transform_box_visible)
        for (curve *_curve : curves)
            for (key_frame *key : _curve->get_keys())
                if (cursor_above_point(cursor_position, key->pos()))
                    this->setCursor(Qt::CrossCursor);
    //
    //

    if (!dragging)
        return;

    this->setCursor(Qt::CrossCursor);

    key_frame *key = drag_key_frame;

    if (drag_handler == 1)
    {
        float angle =
            90 - get_angle_two_points(get_coords(cursor_position), key->pos());

        // Limitacion de handler 1 a 90 grados
        if (angle >= 90 and angle < 180)
            angle = 89.9;
        else if (angle > 180)
            angle = -89.9;
        //
        //

        key->set_left_angle(angle);
        if (!key->is_broken())
            key->set_right_angle(-angle);
    }
    else if (drag_handler == 2)
    {
        float angle =
            90 + get_angle_two_points(get_coords(cursor_position), key->pos());

        // Limitacion de handler 2 a 90 grados
        if (angle >= 90 and angle < 180)
            angle = 89.9;
        else if (angle > 180)
            angle = -89.9;

        key->set_right_angle(angle);
        if (!key->is_broken())
            key->set_left_angle(-angle);
    }
    else
    {
        QPointF coords =
            get_coords(cursor_position) -
            (get_coords(click_position) - key->get_last_position());

        // limitar posicion al key_frame anterior y siguiente
        key_frame *previous_key = get_previous_key(key);
        if (previous_key)
        {
            if (coords.x() < previous_key->x())
                coords.setX(previous_key->x());
        }

        key_frame *next_key = get_next_key(key);
        if (next_key)
        {
            if (coords.x() > next_key->x())
                coords.setX(next_key->x());
        }
        //
        //

        key->set_pos(coords);
        orient_linear_handler(key);
    }

    update();
}

void curve_view::delete_selected_keyframes()
{
    for (key_frame *key: get_selected_keys())
    {
        print(key);
    }
}
