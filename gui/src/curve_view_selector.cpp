#include <curve_view.hpp>

QList<key_frame> curve_view::get_selected_keys()
{
    QList<key_frame> selected;

    for (auto keys : curves)
        for (key_frame key : keys)
            if (key.selected)
                selected.push_back(key);

    return selected;
}

void curve_view::selector_move(QPoint cursor_position)
{
    if (!selecting)
        return;

    selector.setP2(get_coords(cursor_position));
    for (auto &keys : curves)
    {
        for (key_frame &key : keys)
        {
            if (is_point_in_rectangle(key.pos, selector))
                key.selected = true;
            else
                key.selected = false;
        }
    }

    update();
}

void curve_view::resize_box_press(QPoint cursor_position)
{
    QString action = get_resize_action(cursor_position);

    if (!action.isEmpty())
    {
        resize_current_action = action;
        transforming = true;
        last_resize_box = resize_box;
        key_frame_press = get_selected_keys();
    }
}

QLineF curve_view::get_rectangle_of_selected_keyframes()
{
    auto selected = get_selected_keys();

    if (selected.count() <= 1)
        return {};

    key_frame top = selected.first();
    key_frame bottom = selected.first();
    key_frame left = selected.first();
    key_frame right = selected.first();

    for (key_frame key : selected)
    {
        if (key.pos.y() > top.pos.y())
            top = key;

        if (key.pos.y() < bottom.pos.y())
            bottom = key;

        if (key.pos.x() > right.pos.x())
            right = key;

        if (key.pos.x() < left.pos.x())
            left = key;
    }

    QPointF bottom_left = {left.pos.x(), bottom.pos.y()};
    QPointF top_right = {right.pos.x(), top.pos.y()};

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

void curve_view::right_scale(QPointF coords)
{
    resize_box.setP2({coords.x(), resize_box.y2()});

    float last_width = last_resize_box.x2() - last_resize_box.x1();
    float width = resize_box.x2() - resize_box.x1();

    float multiply = width / last_width;
    float x1 = resize_box.x1();

    for (key_frame key : key_frame_press)
    {
        float x = ((key.pos.x() - x1) * multiply) + x1;
        curves[key.curve][key.index].pos.setX(x);
    }
}

void curve_view::resize_box_move(QPoint cursor_position)
{
    if (!show_resize_box)
        return;

    this->setCursor(Qt::ArrowCursor);

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
        QPointF coords = get_coordsf(cursor_position);

        if (action == "right_scale")
            right_scale(coords);
        else if (action == "left_scale")
            resize_box.setP1({coords.x(), resize_box.y1()});
        else if (action == "top_scale")
            resize_box.setP2({resize_box.x2(), coords.y()});
        else if (action == "bottom_scale")
            resize_box.setP1({resize_box.x1(), coords.y()});

        else if (action == "bottom_left_scale")
        {
            resize_box.setP1({coords.x(), resize_box.y1()});
            resize_box.setP1({resize_box.x1(), coords.y()});
        }
        else if (action == "top_right_scale")
        {
            resize_box.setP2({coords.x(), resize_box.y2()});
            resize_box.setP2({resize_box.x2(), coords.y()});
        }
        else if (action == "bottom_right_scale")
        {
            resize_box.setP1({resize_box.x1(), coords.y()});
            resize_box.setP2({coords.x(), resize_box.y2()});
        }
        else if (action == "top_left_scale")
        {
            resize_box.setP1({coords.x(), resize_box.y1()});
            resize_box.setP2({resize_box.x2(), coords.y()});
        }

        update();
    }
}
