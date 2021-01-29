#ifndef SHUFFLE_GUI_H
#define SHUFFLE_GUI_H

#include <QPainter>

#include <node_gui.h>
#include <combo_box.h>
#include <curve_utils.h>

class out_connector : public QWidget
{
private:
	QPoint position;
	const QColor color;
	const QString layer;
	const int index;
	bool fill; // relleno de circulo
	QPushButton *black_button;
	QPushButton *white_button;
public:
	out_connector();
	~out_connector();
};


class in_connector : public QWidget
{
private:
	bool connected;
	bool dragging;
	QPoint position;
	const QColor color;
	const QString layer;

	QList <out_connector*> outputs;
public:
	in_connector();
	~in_connector();

	void disconnect(QString out_layer, int out_index);
};

class in_layer : public QWidget
{
public:
	in_layer(QString name);
	~in_layer();
};

class out_layer : public QWidget
{
public:
	out_layer(QString name);
	~out_layer();
};

class shuffle_gui : public node_gui
{
private:
	QWidget *connection_viewer;
	QLine line;

	// capas
	in_layer *in_layer_a;
	in_layer *in_layer_b;
	out_layer *out_layer_a;
	out_layer *out_layer_b;
	//

	QPoint mouse_position;
	bool dragging;

	void init_connectors();
	void draw_bezier(QPainter &painter, QPoint src, QPoint dst);
	void connect_channel(QString in_layer, int in_index, QString out_layer, int out_index);
	void disconnect_channel(QString in_layer, int in_index, QString out_layer, int out_index);
	void restore_connections();
	int get_output_index(QString layer, QPoint position);
	int get_input_index(QString layer, QPoint position);
	void set_bw_button(QString layer, int index, bool black, bool white);
public:
	shuffle_gui(QVBoxLayout *controls_layout);
	~shuffle_gui();

	void changed(QString param_name) override;

protected:
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
};

#endif // SHUFFLE_GUI_H
