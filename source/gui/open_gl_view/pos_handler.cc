#include <gl_view.h>

void gl_view::pos_handler_draw()
{
    QColor color = {200, 200, 200};
    int size = 6;
    int smooth = false;

	for (auto &handler : pos_handlers)
		draw_point(handler.position, color, size, smooth);
}

void gl_view::pos_handler_update(QString name, QPoint position)
{
	pos_handler_struct handler;
	handler.name = name;
	handler.position = position;
	handler.moving = false;

	pos_handlers.insert(name, handler);
}

void gl_view::pos_handler_clear()
{
	pos_handlers.clear();
}

void gl_view::pos_handler_translate(QPoint cursor_position, pos_handler_struct &handler)
{
	handler.position = get_coords(cursor_position).toPoint();
}

void gl_view::pos_handler_press(QPoint cursor_position)
{
	for (auto &handler : pos_handlers)
	{
		if ( is_cursor_above(cursor_position, handler.position) )
		{
			handler.moving = true;
			break;
		}
	}
}

void gl_view::pos_handler_release(QPoint cursor_position)
{
	for (auto &handler : pos_handlers)
	{
		if (handler.moving)
		{
			pos_handler_translate(cursor_position, handler);
			pos_handler_changed(handler.name, handler.position, true);
		}

		handler.moving = false;
	}
}

void gl_view::pos_handler_move(QPoint cursor_position)
{
	for (auto &handler : pos_handlers)
	{
		if (handler.moving)
		{
			pos_handler_translate(cursor_position, handler);
			pos_handler_changed(handler.name, handler.position);
			update();
		}
	}
}

void gl_view::pos_handler_changed(QString name, QPoint position, bool release){}
