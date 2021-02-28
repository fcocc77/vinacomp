#include <gl_view.h>

void gl_view::box_handler_init()
{
}

void gl_view::add_box_handler(QString name, QRect box)
{
	handler_struct handler;

	handler.transforming = false;
	handler.name = name;
	handler.box.setLine(
		box.x(), box.y(),
		box.x() + box.width(),
		box.y() + box.height()
	);

	handlers.insert(name, handler);
}

void gl_view::clear_box_handler()
{
	handlers.clear();
}

void gl_view::remove_box_handler(QString name)
{
	handlers.remove(name);
}

void gl_view::box_handlers_draw()
{
	for (auto handler : handlers)
		box_handler_draw(handler.box);
}

void gl_view::box_handler_draw(QLineF box_handler)
{
    QColor color = {200, 200, 200};

    QPointF bottom_left = box_handler.p1();
    QPointF top_right = box_handler.p2();
    QPointF bottom_right = {top_right.x(), bottom_left.y()};
    QPointF top_left = {bottom_left.x(), top_right.y()};

    draw_box({bottom_left, top_right}, {10, 10, 10}, color);
    //
    //

    // Vertices
    int size = 6;
    int smooth = false;
    draw_point(bottom_left, color, size, smooth);
    draw_point(bottom_right, color, size, smooth);
    draw_point(top_left, color, size, smooth);
    draw_point(top_right, color, size, smooth);
    //
    //

    // + Central
    float center_x = (bottom_left.x() + bottom_right.x()) / 2;
    float center_y = (bottom_right.y() + top_right.y()) / 2;

    QPointF center = {center_x, center_y};
    center = get_position(center);

    int distance = 30;
    {
        // Linea Horizontal
        QPointF horizontal_p1 = {center.x() - distance, center.y()};
        QPointF horizontal_p2 = {center.x() + distance, center.y()};

        horizontal_p1 = get_coordsf(horizontal_p1);
        horizontal_p2 = get_coordsf(horizontal_p2);

        draw_line(horizontal_p1, horizontal_p2, color);
        //
    }

    {
        // Linea Vertical
        QPointF vertical_p1 = {center.x(), center.y() - distance};
        QPointF vertical_p2 = {center.x(), center.y() + distance};

        vertical_p1 = get_coordsf(vertical_p1);
        vertical_p2 = get_coordsf(vertical_p2);

        draw_line(vertical_p1, vertical_p2, color);
        //
    }

    //
    //
}

QString gl_view::get_transform_action(QPoint cursor_position, handler_struct &handler)
{
    // Obtiene la accion del 'box_handler' a partir del cursor del mouse
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

    QPointF bottom_left = handler.box.p1();
    QPointF top_right = handler.box.p2();
    QPointF bottom_right = {top_right.x(), bottom_left.y()};
    QPointF top_left = {bottom_left.x(), top_right.y()};

    // si las posiciones x o y de la caja de tranformacion son iguales, entoces solo se puede mover
    if (handler.box.x1() == handler.box.x2() || handler.box.y1() == handler.box.y2())
        if (is_cursor_above(cursor_position, handler.box.p1(), handler.box.p2()))
            return "center_translate";
    //

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

void gl_view::box_handler_transform(QPoint cursor_position, handler_struct &handler)
{
    // return;
    QString action = handler.resize_current_action;

    QPointF click_coords = get_coordsf(click_position);
    QPointF coords = get_coordsf(cursor_position);
    QPointF add_translate = coords - click_coords;

    if (action == "right_scale")
    {
        handler.box.setP2({coords.x(), handler.box.y2()});
    }
    else if (action == "left_scale")
    {
        handler.box.setP1({coords.x(), handler.box.y1()});
    }
    else if (action == "top_scale")
    {
        handler.box.setP2({handler.box.x2(), coords.y()});
    }
    else if (action == "bottom_scale")
    {
        handler.box.setP1({handler.box.x1(), coords.y()});
    }

    else if (action == "bottom_left_scale")
    {
        handler.box.setP1({coords.x(), handler.box.y1()});
        handler.box.setP1({handler.box.x1(), coords.y()});
    }
    else if (action == "top_right_scale")
    {
        handler.box.setP2({coords.x(), handler.box.y2()});
        handler.box.setP2({handler.box.x2(), coords.y()});
    }
    else if (action == "bottom_right_scale")
    {
        handler.box.setP1({handler.box.x1(), coords.y()});
        handler.box.setP2({coords.x(), handler.box.y2()});
    }
    else if (action == "top_left_scale")
    {
        handler.box.setP1({coords.x(), handler.box.y1()});
        handler.box.setP2({handler.box.x2(), coords.y()});
    }

    else if (action == "center_translate")
    {
        handler.box.setP1(handler.last_box.p1() + add_translate);
        handler.box.setP2(handler.last_box.p2() + add_translate);
    }
    else if (action == "horizontal_translate")
    {
        QPointF p1 = {handler.last_box.x1() + add_translate.x(), handler.last_box.y1()};
        QPointF p2 = {handler.last_box.x2() + add_translate.x(), handler.last_box.y2()};

        handler.box.setP1(p1);
        handler.box.setP2(p2);
    }
    else if (action == "vertical_translate")
    {
        QPointF p1 = {handler.last_box.x1(), handler.last_box.y1() + add_translate.y()};
        QPointF p2 = {handler.last_box.x2(), handler.last_box.y2() + add_translate.y()};

        handler.box.setP1(p1);
        handler.box.setP2(p2);
    }
}

void gl_view::box_handler_press(QPoint cursor_position)
{
	for (auto &handler : handlers)
	{
		QString action = get_transform_action(cursor_position, handler);

		if (!action.isEmpty())
		{
			handler.resize_current_action = action;
			handler.last_box = handler.box;
			handler.transforming = true;

			break;
		}
	}
}

void gl_view::box_handler_release()
{
	for (auto &handler : handlers)
	{
		if (handler.transforming)
		{
			QRect box(
				handler.box.x1(),
				handler.box.y1(),
				handler.box.x2() - handler.box.x1(),
				handler.box.y2() - handler.box.y1()
			);

			box_handler_finished(box, handler.name);
		}

		handler.transforming = false;
	}
}

void gl_view::box_handler_move(QPoint cursor_position)
{
	bool transforming = false;
	bool over = false;
	for (auto &handler : handlers)
	{
		QString action = get_transform_action(cursor_position, handler);
		if (!over) over = !action.isEmpty();

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

		if (handler.transforming)
		{
			box_handler_transform(cursor_position, handler);
			QRect box(
				handler.box.x1(),
				handler.box.y1(),
				handler.box.x2() - handler.box.x1(),
				handler.box.y2() - handler.box.y1()
			);

			box_handler_changed(box, handler.name);
			transforming = true;
		}
	}
	if (!over)
		this->setCursor(Qt::ArrowCursor);

	if (transforming)
		update();
}

void gl_view::box_handler_changed(QRect box, QString name) {}
void gl_view::box_handler_finished(QRect box, QString name) {}
