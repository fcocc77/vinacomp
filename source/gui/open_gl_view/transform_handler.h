protected:
struct tf_handler_struct
{
    QString name;
    bool transforming;
    QPointF translate;
    float rotate;
    QPointF scale;
    QString action;

    QLineF rotate_handler;

    QLineF x_handler;
    QLineF y_handler;

    QPointF x1_scale_handler;
    QPointF x2_scale_handler;
    QPointF y1_scale_handler;
    QPointF y2_scale_handler;
};

void tf_handler_draw();
void tf_handler_rotate_update(QString name, float rotate);
void tf_handler_translate_update(QString name, QPointF translate);
void tf_handler_scale_update(QString name, float x, float y);
void tf_handler_clear();

virtual void tf_handler_changed(tf_handler_struct handler, bool release = false);

private:
QMap<QString, tf_handler_struct> tf_handlers;
tf_handler_struct handler_click;

void tf_handler_translate(QPoint cursor_position, tf_handler_struct &handler);
void tf_handler_translate_axis(QPoint cursor_position, tf_handler_struct &handler,
                               bool x_axis = true);
void tf_handler_rotate(QPoint cursor_position, tf_handler_struct &handler);
void tf_handler_scale_axis(QPoint cursor_position, tf_handler_struct &handler, QString axis);
void tf_handler_add(QString name);

bool horizontal_axis(float rotate) const;

void tf_handler_press(QPoint cursor_position);
void tf_handler_release(QPoint cursor_position);
void tf_handler_move(QPoint cursor_position);
QString tf_get_action(QPoint cursor_position, tf_handler_struct &handler);
