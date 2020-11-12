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
        transforming = true;
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

void curve_view::resize_box_move(QPoint cursor_position)
{
    this->setCursor(Qt::ArrowCursor);

    if (!resizing)
    {
        resize_box = {{0, 0}, {0, 0}};
        return;
    }

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
}
