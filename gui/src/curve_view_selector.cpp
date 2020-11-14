#include <curve_view.hpp>

QList<key_frame *> curve_view::get_selected_keys()
{
    QList<key_frame *> selected;

    for (auto keys : curves)
        for (key_frame *key : keys)
            if (key->selected())
                selected.push_back(key);

    return selected;
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
    if (!selecting)
        return;

    selector.setP2(get_coords(cursor_position));
    for (auto keys : curves)
    {
        for (key_frame *key : keys)
        {
            if (is_point_in_rectangle(key->pos(), selector))
                key->select(true);
            else
                key->select(false);
        }
    }

    update();
}

void curve_view::resize_box_press(QPoint cursor_position)
{
    if (!resize_box_visible)
        return;

    QString action = get_resize_action(cursor_position);

    if (!action.isEmpty())
    {
        resize_current_action = action;
        transforming = true;
        last_resize_box = resize_box;

        selected_key_frames = get_selected_keys();
        for (key_frame *key : selected_key_frames)
            key->stamp_position();
    }
}

QLineF curve_view::get_rectangle_of_selected_keyframes()
{
    auto selected = get_selected_keys();

    if (selected.count() <= 1)
        return {};

    key_frame *top = selected.first();
    key_frame *bottom = selected.first();
    key_frame *left = selected.first();
    key_frame *right = selected.first();

    for (key_frame *key : selected)
    {
        if (key->y() > top->y())
            top = key;

        if (key->y() < bottom->y())
            bottom = key;

        if (key->x() > right->x())
            right = key;

        if (key->x() < left->x())
            left = key;
    }

    QPointF bottom_left = {left->x(), bottom->y()};
    QPointF top_right = {right->x(), top->y()};

    return {bottom_left, top_right};
}

QString curve_view::get_resize_action(QPoint cursor_position)
{
    // Obtiene la accion del 'resize_box' a partir del cursor del mouse
    bool is_above = false;
    auto above = [&](QPointF point, Qt::CursorShape cursor, QPointF point2 = {}) {
        if (is_cursor_above(cursor_position, point, point2))
        {
            this->setCursor(cursor);
            is_above = true;
            return true;
        }

        return false;
    };

    QPointF bottom_left = resize_box.p1();
    QPointF top_right = resize_box.p2();
    QPointF bottom_right = {top_right.x(), bottom_left.y()};
    QPointF top_left = {bottom_left.x(), top_right.y()};

    // vertices
    if (is_cursor_above(cursor_position, bottom_left))
        return "bottom_left_scale";
    if (is_cursor_above(cursor_position, top_right))
        return "top_right_scale";
    if (is_cursor_above(cursor_position, bottom_right))
        return "bottom_right_scale";
    if (is_cursor_above(cursor_position, top_left))
        return "top_left_scale";
    //
    //

    // Lineas del border
    QLineF left = {bottom_left, top_left};
    QLineF right = {bottom_right, top_right};
    QLineF bottom = {bottom_left, bottom_right};
    QLineF top = {top_left, top_right};

    if (is_cursor_above(cursor_position, left.p1(), left.p2()))
        return "left_scale";
    if (is_cursor_above(cursor_position, right.p1(), right.p2()))
        return "right_scale";
    if (is_cursor_above(cursor_position, bottom.p1(), bottom.p2()))
        return "bottom_scale";
    if (is_cursor_above(cursor_position, top.p1(), top.p2()))
        return "top_scale";
    //
    //

    // Lineas centrales
    int distance = 30;

    float center_x = (bottom_left.x() + bottom_right.x()) / 2;
    float center_y = (bottom_right.y() + top_right.y()) / 2;

    QPointF center = {center_x, center_y};
    center = get_position(center);

    QPointF horizontal_p1 = get_coordsf({center.x() - distance, center.y()});
    QPointF horizontal_p2 = get_coordsf({center.x() + distance, center.y()});
    bool horizontal = is_cursor_above(cursor_position, horizontal_p1, horizontal_p2);

    QPointF vertical_p1 = get_coordsf({center.x(), center.y() - distance});
    QPointF vertical_p2 = get_coordsf({center.x(), center.y() + distance});
    bool vertical = is_cursor_above(cursor_position, vertical_p1, vertical_p2);

    if (vertical && horizontal)
        return "center_translate";
    else if (vertical)
        return "vertical_translate";
    else if (horizontal)
        return "horizontal_translate";
    //
    //

    return "";
}

void curve_view::scale_key_from_point(QPointF point)
{
    float last_width = last_resize_box.x2() - last_resize_box.x1();
    float width = resize_box.x2() - resize_box.x1();

    float last_height = last_resize_box.y2() - last_resize_box.y1();
    float height = resize_box.y2() - resize_box.y1();

    float multiply_x = width / last_width;
    float multiply_y = height / last_height;

    for (key_frame *key : selected_key_frames)
    {
        float x = ((key->get_last_position().x() - point.x()) * multiply_x) + point.x();
        float y = ((key->get_last_position().y() - point.y()) * multiply_y) + point.y();

        key->set_pos({x, y});
    }
}

void curve_view::translate_keys(QPointF add_translate)
{
    for (key_frame *key : selected_key_frames)
        key->set_pos(key->get_last_position() + add_translate);
}

void curve_view::resize_box_move(QPoint cursor_position)
{
    if (!resize_box_visible)
        return;

    QString action = get_resize_action(cursor_position);

    if (action == "bottom_left_scale" || action == "top_right_scale")
        this->setCursor(Qt::SizeBDiagCursor);
    else if (action == "bottom_right_scale" || action == "top_left_scale")
        this->setCursor(Qt::SizeFDiagCursor);
    else if (action == "left_scale" || action == "right_scale" || action == "horizontal_translate")
        this->setCursor(Qt::SizeHorCursor);
    else if (action == "top_scale" || action == "bottom_scale" || action == "vertical_translate")
        this->setCursor(Qt::SizeVerCursor);
    else if (action == "center_translate")
        this->setCursor(Qt::SizeAllCursor);

    if (transforming)
    {
        action = resize_current_action;
        QPointF click_coords = get_coordsf(click_position);
        QPointF coords = get_coordsf(cursor_position);
        QPointF add_translate = coords - click_coords;

        // Limitacion en x hasta el key frame anterior y siguiente de la seleccion de keyframes.
        key_frame *previous_key = get_previous_key(selected_key_frames.first());
        key_frame *next_key = get_next_key(selected_key_frames.last());

        if (previous_key)
        {
            // Limitacion para 'scale'
            if (coords.x() < previous_key->x())
                coords.setX(previous_key->x());

            // Limitacion para 'translate'
            float left_border = last_resize_box.x1() + add_translate.x();
            if (left_border < previous_key->x())
                add_translate.setX(previous_key->x() - last_resize_box.x1());
        }

        if (next_key)
        {
            // Limitacion para 'scale'
            if (coords.x() > next_key->x())
                coords.setX(next_key->x());

            // Limitacion para 'translate'
            float right_border = last_resize_box.x2() + add_translate.x();
            if (right_border > next_key->x())
                add_translate.setX(next_key->x() - last_resize_box.x2());
        }
        //
        //

        // Limitacion para que no traspase el borde izquierdo
        if (action == "right_scale" || action == "top_right_scale" || action == "bottom_right_scale")
            if (coords.x() < resize_box.x1())
                coords.setX(resize_box.x1());

        // Limitacion para que no traspase el borde derecho
        if (action == "left_scale" || action == "top_left_scale" || action == "bottom_left_scale")
            if (coords.x() > resize_box.x2())
                coords.setX(resize_box.x2());

        // Limitacion para que no traspase el borde superior
        if (action == "bottom_scale" || action == "bottom_left_scale" || action == "bottom_right_scale")
            if (coords.y() > resize_box.y2())
                coords.setY(resize_box.y2());

        // Limitacion para que no traspase el borde inferior
        if (action == "top_scale" || action == "top_right_scale" || action == "top_left_scale")
            if (coords.y() < resize_box.y1())
                coords.setY(resize_box.y1());
        //
        //

        if (action == "right_scale")
        {
            resize_box.setP2({coords.x(), resize_box.y2()});
            scale_key_from_point({resize_box.x1(), 0});
        }
        else if (action == "left_scale")
        {
            resize_box.setP1({coords.x(), resize_box.y1()});
            scale_key_from_point({resize_box.x2(), 0});
        }
        else if (action == "top_scale")
        {
            resize_box.setP2({resize_box.x2(), coords.y()});
            scale_key_from_point({0, resize_box.y1()});
        }
        else if (action == "bottom_scale")
        {
            resize_box.setP1({resize_box.x1(), coords.y()});
            scale_key_from_point({0, resize_box.y2()});
        }

        else if (action == "bottom_left_scale")
        {
            resize_box.setP1({coords.x(), resize_box.y1()});
            resize_box.setP1({resize_box.x1(), coords.y()});
            scale_key_from_point({resize_box.x2(), resize_box.y2()});
        }
        else if (action == "top_right_scale")
        {
            resize_box.setP2({coords.x(), resize_box.y2()});
            resize_box.setP2({resize_box.x2(), coords.y()});
            scale_key_from_point({resize_box.x1(), resize_box.y1()});
        }
        else if (action == "bottom_right_scale")
        {
            resize_box.setP1({resize_box.x1(), coords.y()});
            resize_box.setP2({coords.x(), resize_box.y2()});
            scale_key_from_point({resize_box.x1(), resize_box.y2()});
        }
        else if (action == "top_left_scale")
        {
            resize_box.setP1({coords.x(), resize_box.y1()});
            resize_box.setP2({resize_box.x2(), coords.y()});
            scale_key_from_point({resize_box.x2(), resize_box.y1()});
        }

        else if (action == "center_translate")
        {
            resize_box.setP1(last_resize_box.p1() + add_translate);
            resize_box.setP2(last_resize_box.p2() + add_translate);
            translate_keys(add_translate);
        }
        else if (action == "horizontal_translate")
        {
            QPointF p1 = {last_resize_box.x1() + add_translate.x(), last_resize_box.y1()};
            QPointF p2 = {last_resize_box.x2() + add_translate.x(), last_resize_box.y2()};

            resize_box.setP1(p1);
            resize_box.setP2(p2);

            translate_keys({add_translate.x(), 0});
        }
        else if (action == "vertical_translate")
        {
            QPointF p1 = {last_resize_box.x1(), last_resize_box.y1() + add_translate.y()};
            QPointF p2 = {last_resize_box.x2(), last_resize_box.y2() + add_translate.y()};

            resize_box.setP1(p1);
            resize_box.setP2(p2);

            translate_keys({0, add_translate.y()});
        }

        update();
    }
}
