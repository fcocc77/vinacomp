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
	int ch_output;
};

struct out_connector
{
	bool connected;
	QPoint position;
	QColor color;
	int ch_input;
	bool fill; // relleno de circulo
};

class shuffle_gui : public node_gui
{
private:
	QWidget *connection_viewer;
	QLine line;

	QWidget *input_a, *input_b, *output_a, *output_b;
	QList <in_connector> inputs_a, inputs_b;
	QList <out_connector> outputs_a, outputs_b;

	QPoint mouse_position;

	void init_connectors();
	QWidget *create_input();
	QWidget *create_output();
	void draw_bezier(QPainter &painter, QPoint src, QPoint dst);
	void connect_channel(int in_index, int out_index);
	void disconnect_channel(int in_index);
	void restore_connections();
	int get_output_index(QPoint position);
	int get_input_index(QPoint position);
public:
	shuffle_gui(QVBoxLayout *controls_layout);
	~shuffle_gui();

	void changed(QString param_name) override;

protected:
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
};

#endif // SHUFFLE_GUI_H
