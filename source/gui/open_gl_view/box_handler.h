public:
struct box_handler_struct
{
    QString name;
    QString type;
    QLineF box;
    QLineF last_box;
    QString resize_current_action;
    bool transforming;
};

void box_handlers_draw();
void box_handler_update(QString name, QString type, QRect box);
void box_handler_clear();

private:
QMap<QString, box_handler_struct> handlers;

void box_handler_transform(QPoint cursor_position, box_handler_struct &handler);
QString get_transform_action(QPoint cursor_position, box_handler_struct &handler);
void box_handler_draw(QLineF box_handler);

void box_handler_press(QPoint cursor_position);
void box_handler_release(QPoint cursor_position);
void box_handler_move(QPoint cursor_position);

protected:
virtual void box_handler_changed(QString name, QString type, QRect box, bool release = false);
