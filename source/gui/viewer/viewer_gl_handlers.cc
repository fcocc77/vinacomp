#include <viewer_gl.h>
#include <properties.h>
#include <trim_panel.h>

void viewer_gl::handlers_update()
{
	panels = dynamic_cast< properties * >(_properties)->get_trim_panels();

	box_handler_clear();
	pos_handler_clear();

	for (QWidget *_panel : panels)
	{
		trim_panel *panel = dynamic_cast<trim_panel *>(_panel);

		QString type = panel->get_type();
		QString name = panel->get_name();

		if (type == "crop")
		{
			knob_intd *box_knob = dynamic_cast <knob_intd*>(panel->get_knob("box"));
			auto values = box_knob->get_values();
			box_handler_add( name, { values[0], values[1], values[2], values[3] });
		}

		else if (type == "position")
		{
			knob_intd *translate = dynamic_cast <knob_intd*>(panel->get_knob("translate"));
			auto values = translate->get_values();
			pos_handler_add(name, {values[0], values[1]});
		}
	};
	update();
}

void viewer_gl::update_handler(QString node_name, QString param_name)
{
	// actualizar el manejador especifico en gl_view
	print(node_name + " : " + param_name);
}

void viewer_gl::draw_handlers()
{
	box_handlers_draw();
	pos_handler_draw();
}

knob *viewer_gl::get_knob(QString panel_name, QString knob_name)
{
	QWidget *_panel = dynamic_cast<properties*>(_properties)->get_trim_panel(panel_name);
	trim_panel *panel = dynamic_cast<trim_panel *>(_panel);

	return panel->get_knob(knob_name);
}

void viewer_gl::box_handler_changed(QString name, QRect box, bool release)
{
	knob_intd *box_knob = dynamic_cast<knob_intd*>(get_knob(name, "box"));
	box_knob->set_values({box.x(), box.y(), box.width(), box.height()}, false);
	if (release)
		box_knob->emmit_signal();
}

void viewer_gl::pos_handler_changed(QString name, QPoint position, bool release)
{
	knob_intd *translate = dynamic_cast<knob_intd*>(get_knob(name, "translate"));
	translate->set_values({position.x(), position.y()}, false);
	if (release)
		translate->emmit_signal();
}

