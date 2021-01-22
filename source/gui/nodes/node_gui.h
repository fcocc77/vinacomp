#ifndef NODE_GUI_H
#define NODE_GUI_H

#include <QWidget>

#include <knob.h>
#include <util.h>

class node_gui
{
private:
	QWidget *_vinacomp;
	QWidget *_trim_panel;
public:
	node_gui();
	~node_gui();

	void setup(QWidget *_trim_panel, QWidget *_vinacomp);
	virtual void clicked();

protected:
	knob *get_knob(QString name) const;
};

#endif // NODE_GUI_H
