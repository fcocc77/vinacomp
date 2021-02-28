
private:
	struct handler_struct
	{
		QString name;
		QLineF box;
		QLineF last_box;
		QString resize_current_action;
		bool transforming;
	};

	QMap <QString, handler_struct> handlers;

	void box_handler_init();
	void box_handler_transform(QPoint cursor_position, handler_struct &handler);
	QString get_transform_action(QPoint cursor_position, handler_struct &handler);
	void box_handler_draw(QLineF box_handler);

	void box_handler_press(QPoint cursor_position);
	void box_handler_release();
	void box_handler_move(QPoint cursor_position);

public:
	void box_handlers_draw();
	void add_box_handler(QString name, QRect box);
	void remove_box_handler(QString name);
	void clear_box_handler();

protected:
	virtual void box_handler_changed(QRect box, QString name);
	virtual void box_handler_finished(QRect box, QString name);
