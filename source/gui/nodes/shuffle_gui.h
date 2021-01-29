#ifndef SHUFFLE_GUI_H
#define SHUFFLE_GUI_H

#include <QPainter>

#include <node_gui.h>
#include <combo_box.h>
#include <curve_utils.h>
#include <util.h>

class connector : public QWidget
{
private:
	QPoint position;
	const QColor color;
	const QString layer;
public:
	connector(QString layer, QColor color);

	QColor get_color() const;
	QPoint get_position() const;
	void set_position(QPoint _position);
	bool is_connected() const;
	void set_connected(bool _connected);
protected:
	bool connected;
};

class out_connector : public connector
{
private:
	connector *in_conn;
	QPushButton *black_button;
	QPushButton *white_button;
public:
	out_connector(QString layer, QString label, QColor color);
	~out_connector();

	void connect_input(connector *in_conn);
	void disconnect();
};


class in_connector : public connector
{
private:
	QList <out_connector*> outputs;
public:
	in_connector(QString layer, QString label, QColor color);
	~in_connector();

	QList <out_connector*> get_outputs() const;
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

	QList<in_connector*> get_connectors() const;
};

class out_layer : public QWidget
{
private:
	out_connector *red_connector;
	out_connector *green_connector;
	out_connector *blue_connector;
	out_connector *alpha_connector;
public:
	out_layer(QString layer);
	~out_layer();

	QList<out_connector*> get_connectors() const;
};

class shuffle_gui : public node_gui
{
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

	void draw_bezier(QPainter &painter, QPoint src, QPoint dst);
	in_connector *get_in_connector(QPoint position) const;
	out_connector *get_out_connector(QPoint position) const;
public:
	shuffle_gui(QVBoxLayout *controls_layout);
	~shuffle_gui();


protected:
	void paintEvent(QPaintEvent *event) override;
	// void mouseDoubleClickEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
};

#endif // SHUFFLE_GUI_H
