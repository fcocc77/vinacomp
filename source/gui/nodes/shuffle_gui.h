#ifndef SHUFFLE_GUI_H
#define SHUFFLE_GUI_H

#include <QPainter>

#include <node_gui.h>
#include <combo_box.h>
#include <curve_utils.h>

struct in_connector
{
	bool connected;
	bool dragging;
	QPoint position;
	QColor color;
	const QString layer;
	QString out_layer;
	int output;
};

struct out_connector
{
	bool connected;
	QPoint position;
	QColor color;
	QString in_layer;
	int input;
	bool fill; // relleno de circulo
	QPushButton *black_button;
	QPushButton *white_button;
};

class shuffle_gui : public node_gui
{
private:
	QWidget *connection_viewer;
	QLine line;

	QWidget *input_a, *input_b, *output_a, *output_b;
	QMap <QString, QList <in_connector>> inputs;
	QMap <QString, QList <out_connector>> outputs;

	QPoint mouse_position;
	bool dragging;

	void init_connectors();
	QWidget *create_input();
	QWidget *create_output(QString layer);
	void draw_bezier(QPainter &painter, QPoint src, QPoint dst);
	void connect_channel(QString in_layer, int in_index, QString out_layer, int out_index);
	void disconnect_channel(QString layer, int in_index);
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
