
private:
	QLineF box_handler;
    QLineF last_box_handler;

	QString resize_current_action;
	bool transforming;
	bool box_handler_visible;

	void box_handler_init();
	void box_handler_transform(QPoint cursor_position);
	void box_handler_press(QPoint cursor_position);
	void box_handler_move(QPoint cursor_position);
	QString get_transform_action(QPoint cursor_position);

public:
	void box_handler_draw();
