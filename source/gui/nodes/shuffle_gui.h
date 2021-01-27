#ifndef SHUFFLE_GUI_H
#define SHUFFLE_GUI_H

#include <QPainter>

#include <node_gui.h>
#include <combo_box.h>

class shuffle_gui : public node_gui
{
private:
	QWidget *create_input();
	void create_input_channel(QVBoxLayout *parent_layout, QString label, QString color_name);

	QWidget *create_output();
	void create_output_channel(QVBoxLayout *parent_layout, QString label, QString color_name);
public:
	shuffle_gui(QVBoxLayout *controls_layout);
	~shuffle_gui();

	void changed(QString param_name) override;

	void paintEvent(QPaintEvent*) override;
};

#endif // SHUFFLE_GUI_H
