#ifndef SHUFFLE_GUI_H
#define SHUFFLE_GUI_H

#include <QPainter>
#include <QMouseEvent>
#include <QWidget>

#include <node_gui.h>

class connector : public QWidget
{
private:
    QPoint position;
    const QColor color;
    const QString layer;
    const int index;

public:
    connector(QString layer, int _index, QColor color);

    QColor get_color() const;
    QString get_layer() const;
    QPoint get_position() const;
    int get_index() const;
    void set_position(QPoint _position);
    bool is_connected() const;
    void set_connected(bool _connected);

protected:
    bool connected;
};

class out_connector : public connector
{
private:
    node_gui *parent;
    connector *in_conn;
    QPushButton *black_button;
    QPushButton *white_button;
    QLabel *channel_label;

    bool black, white;

public:
    out_connector(node_gui *parent, QString layer, int index, QString label, QColor color);
    ~out_connector();

    void set_bw_button(bool _black, bool _white);
    void connect_input(connector *in_conn);
    void disconnect();

    int get_state() const;
    connector *get_in_connector() const;
    QString get_input_layer() const;
};

class in_connector : public connector
{
private:
    QList<out_connector *> outputs;

public:
    in_connector(QString layer, int index, QString label, QColor color);
    ~in_connector();

    QList<out_connector *> get_outputs() const;
    void connect_output(out_connector *out_conn);
    void disconnect(out_connector *out_conn);
};

class in_layer : public QWidget
{
private:
    in_connector *red_connector;
    in_connector *green_connector;
    in_connector *blue_connector;
    in_connector *alpha_connector;

public:
    in_layer(QString layer);
    ~in_layer();

    QList<in_connector *> get_connectors() const;
};

class out_layer : public QWidget
{
private:
    out_connector *red_connector;
    out_connector *green_connector;
    out_connector *blue_connector;
    out_connector *alpha_connector;

public:
    out_layer(node_gui *parent, QString layer);
    ~out_layer();

    QList<out_connector *> get_connectors() const;
};

class shuffle_gui : public node_gui
{
    Q_OBJECT
private:
    QWidget *connection_viewer;

    // capas
    in_layer *in_layer_a;
    in_layer *in_layer_b;
    out_layer *out_layer_a;
    out_layer *out_layer_b;
    //

    in_connector *dragging_input;
    QPoint mouse_position;
    bool dragging;
    QJsonObject last_data;

    QJsonObject get_data() const;
    void restore_connections(QJsonObject data);
    void draw_bezier(QPainter &painter, QPoint src, QPoint dst);
    in_connector *get_in_connector(QPoint position) const;
    out_connector *get_out_connector(QPoint position) const;
    void to_connect(in_connector *in_conn, out_connector *out_conn);
    void restore_param();
    void setup_knobs(QMap<QString, QVBoxLayout *> layouts) override;

public:
    shuffle_gui();
    ~shuffle_gui();

    void emmit_signal();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // SHUFFLE_GUI_H
