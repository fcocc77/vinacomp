
private:
	struct handler_struct
	{
		QString name;
		QLineF box;
		QLineF last_box;
		QString resize_current_action;
		bool transforming;
		bool visible;
	};

	QMap <QString, handler_struct> handlers;

	void box_handler_init();
	void box_handler_transform(QPoint cursor_position, handler_struct &handler);
	void box_handler_press(QPoint cursor_position);
	void box_handler_move(QPoint cursor_position);
	QString get_transform_action(QPoint cursor_position, handler_struct &handler);
	void box_handler_draw(QLineF box_handler);

public:
	void box_handlers_draw();

protected:
	virtual void box_handler_changed(QRect box, QString name);
