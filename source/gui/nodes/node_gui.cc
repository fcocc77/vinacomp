#include <node_gui.h>
#include <trim_panel.h>

node_gui::node_gui()
	: _vinacomp(nullptr)
	, _trim_panel(nullptr)
{
}

node_gui::~node_gui(){}

void node_gui::setup(QWidget *__trim_panel, QWidget *__vinacomp)
{
	_vinacomp = __vinacomp;
	_trim_panel = __trim_panel;
}

void node_gui::clicked()
{
}

knob *node_gui::get_knob(QString name) const
{
	trim_panel *panel = dynamic_cast<trim_panel*>(_trim_panel);
	return panel->get_knob(name);
}
