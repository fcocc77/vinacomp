#include <gl_view.h>

void gl_view::tf_handler_draw()
{
    QColor color = {200, 200, 200};
    int size = 6;
    int smooth = false;

	int angle = 0;
	QPoint translate = { 200, 200 };
	int handler_ratio = 200;

	QPointF skew_h1 = arc_point(translate, handler_ratio, angle + 180);
	QPointF skew_h2 = arc_point(translate, handler_ratio, angle);
	QPointF skew_v1 = arc_point(translate, handler_ratio, angle - 90);
	QPointF skew_v2 = arc_point(translate, handler_ratio, angle + 90);


	QPointF rotate_1 = arc_point(translate, handler_ratio, angle + 180);
	QPointF rotate_2 = arc_point(translate, handler_ratio + 200, angle);

	for (auto &handler : tf_handlers)
	{
		draw_line(skew_h1, skew_h2, color);
		draw_line(skew_v1, skew_v2, color);

		draw_line(rotate_1, rotate_2, color);
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

void gl_view::tf_handler_press(QPoint cursor_position)
{
	for (auto &handler : tf_handlers)
	{
		if ( is_cursor_above(cursor_position, handler.position) )
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
	for (auto &handler : tf_handlers)
	{
		if (handler.moving)
		{
			tf_handler_translate(cursor_position, handler);
			tf_handler_changed(handler.name, handler.position);
			update();
		}
	}
}

void gl_view::tf_handler_changed(QString name, QPoint position, bool release){}
