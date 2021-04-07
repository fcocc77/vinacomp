#include <curve_view.h>
#include <util.h>

bool curve_view::is_over_cursor(QPoint cursor_position)
{
    int frame = project->frame;

    QPointF up_cursor = {float(frame), get_coordsf({0, 0}).y()};
    QPointF down_cursor = {float(frame), get_coordsf({0, (float)height()}).y()};

    if (cursor_above_point(cursor_position, up_cursor) ||
        cursor_above_point(cursor_position, down_cursor))
        return true;

    return false;
}

void curve_view::cursor_press(QPoint cursor_position)
{
    if (!player_cursor_visible)
        return;

    if (is_over_cursor(cursor_position))
        dragging_cursor = true;
}

void curve_view::cursor_move(QPoint cursor_position)
{
    if (!player_cursor_visible)
        return;

    if (is_over_cursor(cursor_position))
        this->setCursor(Qt::SizeHorCursor);

    if (!dragging_cursor)
        return;

    this->setCursor(Qt::SizeHorCursor);

    project->frame = round(get_coords(cursor_position).x());
    update();
}

void curve_view::set_cursor_visibility(bool visible)
{
    player_cursor_visible = visible;
    update();
}
