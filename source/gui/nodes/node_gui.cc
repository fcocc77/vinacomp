#include <node_gui.h>
#include <vinacomp.h>
#include <trim_panel.h>

node_gui::node_gui()
	: _trim_panel(nullptr)
	, _renderer(nullptr)
	, project(nullptr)
{
}

node_gui::~node_gui(){}

void node_gui::setup(QWidget *__trim_panel, QWidget *_vinacomp, QString _name)
{
	_trim_panel = __trim_panel;
	name = _name;
	vinacomp *__vinacomp = dynamic_cast<vinacomp*>(_vinacomp);

	_renderer = __vinacomp->get_renderer();
	project = __vinacomp->get_project();
}

void node_gui::changed(QString param_name)
{
}

knob *node_gui::get_knob(QString name) const
{
	trim_panel *panel = dynamic_cast<trim_panel*>(_trim_panel);
	return panel->get_knob(name);
}
