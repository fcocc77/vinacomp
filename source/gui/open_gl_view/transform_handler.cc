#include <gl_view.h>

void gl_view::tf_handler_draw()
{
    QColor color = {200, 200, 200};
    int size = 6;
    int smooth = false;

	int handler_ratio = 50;

	for (auto &handler : tf_handlers)
	{
		float angle = handler.rotate;
		QPointF translate_viewport = get_position(handler.translate);

		QPointF skew_h1 = arc_point({0, 0}, handler_ratio, angle + 180);
		QPointF skew_h2 = arc_point({0, 0}, handler_ratio, angle);
		QPointF skew_v1 = arc_point({0, 0}, handler_ratio, angle - 90);
		QPointF skew_v2 = arc_point({0, 0}, handler_ratio, angle + 90);

		skew_h1 = get_coordsf(skew_h1 + translate_viewport);
		skew_h2 = get_coordsf(skew_h2 + translate_viewport);
		skew_v1 = get_coordsf(skew_v1 + translate_viewport);
		skew_v2 = get_coordsf(skew_v2 + translate_viewport);

		QPointF rotate_p1 = arc_point({0, 0}, handler_ratio, angle + 180);
		QPointF rotate_p2 = arc_point({0, 0}, handler_ratio + handler_ratio / 2, angle);

		handler.rotate_handler = {
			get_coordsf(rotate_p1 + translate_viewport),
			get_coordsf(rotate_p2 + translate_viewport)
		};

		draw_line(skew_h1, skew_h2, color);
		draw_line(skew_v1, skew_v2, color);

		draw_line(handler.rotate_handler.p1(), handler.rotate_handler.p2(), color);
		draw_circle(handler.translate, 40);
	}
}

void gl_view::tf_handler_update(QString name, QPoint position)
{
	tf_handler_struct handler;
	handler.name = name;
	handler.translate = {300, 300};
	handler.rotate = 0;
	handler.transforming = false;

	tf_handlers.insert(name, handler);
}

void gl_view::tf_handler_clear()
{
	tf_handlers.clear();
}

void gl_view::tf_handler_translate(QPoint cursor_position, tf_handler_struct &handler)
{
}

void gl_view::tf_handler_rotate(QPoint cursor_position, tf_handler_struct &handler)
{
	QPointF translate = get_position(handler.translate);

    // calcular la rotacion a partir de 2 puntos
    double delta_y = (translate.y() - cursor_position.y());
    double delta_x = (cursor_position.x() - translate.x());

    float rotate = atan2(delta_x, delta_y) * 180 / M_PI;
	handler.rotate = rotate - 90;
}

QString gl_view::tf_get_action(QPoint cursor_position, tf_handler_struct &handler)
{
	QString action = "";

	if ( cursor_above_line(cursor_position, handler.rotate_handler) )
		action = "rotate";

	return action;
}

void gl_view::tf_handler_press(QPoint cursor_position)
{
	for (auto &handler : tf_handlers)
	{
		QString action = tf_get_action(cursor_position, handler);
		if ( !action.isEmpty() )
		{
			handler.transforming = true;
			handler.action = action;
			break;
		}
	}
}

void gl_view::tf_handler_release(QPoint cursor_position)
{
	for (auto &handler : tf_handlers)
	{
		if (handler.transforming)
		{
			tf_handler_translate(cursor_position, handler);
			tf_handler_changed(handler, true);
		}

		handler.transforming = false;
		handler.action = "";
	}
}

void gl_view::tf_handler_move(QPoint cursor_position)
{
	bool over = false;
	for (auto &handler : tf_handlers)
	{
		QString action = tf_get_action(cursor_position, handler);
		if (!over) over = !action.isEmpty();

		if ( action == "rotate" )
			this->setCursor(Qt::CrossCursor);


		if ( handler.action == "rotate" )
			tf_handler_rotate(cursor_position, handler);

		if (handler.transforming)
		{
			tf_handler_changed(handler);
			update();
		}
	}

	if (!over)
		this->setCursor(Qt::ArrowCursor);
}

void gl_view::tf_handler_changed(tf_handler_struct handler, bool release){}
