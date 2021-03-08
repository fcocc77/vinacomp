protected:
	struct tf_handler_struct
	{
		QString name;
		bool transforming;
		QPoint translate;
		float rotate;
		QLineF rotate_handler;
		QString action;
	};

	void tf_handler_draw();
	void tf_handler_rotate_update(QString name, float rotate);
	void tf_handler_translate_update(QString name, QPoint translate);
	void tf_handler_clear();

	virtual void tf_handler_changed(tf_handler_struct handler, bool release = false);

private:
	QMap <QString, tf_handler_struct> tf_handlers;
	void tf_handler_translate(QPoint cursor_position, tf_handler_struct &handler);
	void tf_handler_rotate(QPoint cursor_position, tf_handler_struct &handler);
	void tf_handler_add(QString name);

	void tf_handler_press(QPoint cursor_position);
	void tf_handler_release(QPoint cursor_position);
	void tf_handler_move(QPoint cursor_position);
	QString tf_get_action(QPoint cursor_position, tf_handler_struct &handler);

