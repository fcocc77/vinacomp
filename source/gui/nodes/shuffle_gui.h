#ifndef SHUFFLE_GUI_H
#define SHUFFLE_GUI_H

#include <QPainter>

#include <node_gui.h>
#include <combo_box.h>

struct connectors
{
	bool connected;
	QPoint position;
	QColor color;
};

class shuffle_gui : public node_gui
{
private:
	QWidget *connection_viewer;
	QLine line;

	QWidget *input_a, *input_b, *output_a, *output_b;
	QList <connectors> inputs_a, inputs_b, outputs_a, outputs_b;

	void init_connectors();
	QWidget *create_input();
	QWidget *create_output();
public:
	shuffle_gui(QVBoxLayout *controls_layout);
	~shuffle_gui();

	void changed(QString param_name) override;

protected:
	void paintEvent(QPaintEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // SHUFFLE_GUI_H
