
private:
	struct pos_handler_struct
	{
		QString name;
		QPoint position;
		bool moving;
	};

	QMap <QString, pos_handler_struct> pos_handlers;
	void pos_handler_translate(pos_handler_struct &handler);

	void pos_handler_press(QPoint cursor_position);
	void pos_handler_release();
	void pos_handler_move(QPoint cursor_position);

public:
	void pos_handler_draw();

	void pos_handler_add(QString name, QPoint position);
	void pos_handler_clear();

protected:
	virtual void pos_handler_changed(QString name, QPoint position);
	virtual void pos_handler_finished(QString name, QPoint position);
