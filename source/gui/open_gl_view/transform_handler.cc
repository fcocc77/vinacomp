#include <gl_view.h>

void gl_view::tf_handler_draw()
{
    QColor color = {200, 200, 200};
    int size = 6;
    int smooth = false;

	int angle = 45;
	QPoint translate = { 200, 200 };
	int handler_ratio = 100;


	QPointF translate_viewport = get_position(translate);

	QPointF skew_h1 = arc_point({0, 0}, handler_ratio, angle + 180);
	QPointF skew_h2 = arc_point({0, 0}, handler_ratio, angle);
	QPointF skew_v1 = arc_point({0, 0}, handler_ratio, angle - 90);
	QPointF skew_v2 = arc_point({0, 0}, handler_ratio, angle + 90);

	skew_h1 = get_coordsf(skew_h1 + translate_viewport);
	skew_h2 = get_coordsf(skew_h2 + translate_viewport);
	skew_v1 = get_coordsf(skew_v1 + translate_viewport);
	skew_v2 = get_coordsf(skew_v2 + translate_viewport);

	for (auto &handler : tf_handlers)
	{
		QPointF rotate_p1 = arc_point({0, 0}, handler_ratio, angle + 180);
		QPointF rotate_p2 = arc_point({0, 0}, handler_ratio + 100, angle);

		handler.rotate_handler = {
			get_coordsf(rotate_p1 + translate_viewport),
			get_coordsf(rotate_p2 + translate_viewport)
		};

		draw_line(skew_h1, skew_h2, color);
		draw_line(skew_v1, skew_v2, color);

		draw_line(handler.rotate_handler.p1(), handler.rotate_handler.p2(), color);
	}
}

void gl_view::tf_handler_update(QString name, QPoint position)
{
	tf_handler_struct handler;
	handler.name = name;
	handler.position = position;
	handler.moving = false;

	tf_handlers.insert(name, handler);
}

void gl_view::tf_handler_clear()
{
	tf_handlers.clear();
}

void gl_view::tf_handler_translate(QPoint cursor_position, tf_handler_struct &handler)
{
	handler.position = get_coords(cursor_position).toPoint();
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
			handler.moving = true;
			break;
		}
	}
}

void gl_view::tf_handler_release(QPoint cursor_position)
{
	for (auto &handler : tf_handlers)
	{
		if (handler.moving)
		{
			tf_handler_translate(cursor_position, handler);
			tf_handler_changed(handler.name, handler.position, true);
		}

		handler.moving = false;
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
			this->setCursor(Qt::SizeBDiagCursor);

		if (handler.moving)
		{
			tf_handler_translate(cursor_position, handler);
			tf_handler_changed(handler.name, handler.position);
			update();
		}
	}

	if (!over)
		this->setCursor(Qt::ArrowCursor);
}

void gl_view::tf_handler_changed(QString name, QPoint position, bool release){}
