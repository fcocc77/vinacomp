
private:
	struct tf_handler_struct
	{
		QString name;
		QPoint position;
		bool moving;
		QLineF rotate_handler;
	};

	QMap <QString, tf_handler_struct> tf_handlers;
	void tf_handler_translate(QPoint cursor_position, tf_handler_struct &handler);

	void tf_handler_press(QPoint cursor_position);
	void tf_handler_release(QPoint cursor_position);
	void tf_handler_move(QPoint cursor_position);
	QString tf_get_action(QPoint cursor_position, tf_handler_struct &handler);

public:
	void tf_handler_draw();

	void tf_handler_update(QString name, QPoint position);
	void tf_handler_clear();

protected:
	virtual void tf_handler_changed(QString name, QPoint position, bool release = false);
