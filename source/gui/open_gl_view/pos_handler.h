
public:
struct pos_handler_struct
{
    QString type;
    QString name;
    QPoint position;
    bool moving;
};

void pos_handler_draw();

void pos_handler_update(QString name, QString type, QPoint position);
void pos_handler_clear();

private:
QMap<QString, pos_handler_struct> pos_handlers;
void pos_handler_translate(QPoint cursor_position, pos_handler_struct &handler);

void pos_handler_press(QPoint cursor_position);
void pos_handler_release(QPoint cursor_position);
void pos_handler_move(QPoint cursor_position);

protected:
virtual void pos_handler_changed(pos_handler_struct handler, bool release = false);
