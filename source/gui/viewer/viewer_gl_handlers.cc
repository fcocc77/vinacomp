#include <viewer_gl.h>
#include <properties.h>
#include <trim_panel.h>

void viewer_gl::handlers_update()
{
	panels = static_cast< properties * >(_properties)->get_trim_panels();

	box_handler_clear();
	pos_handler_clear();

	for (QWidget *_panel : panels)
	{
		trim_panel *panel = static_cast<trim_panel *>(_panel);
		if (panel->maximized())
		{
			QString type = panel->get_type();

			if (type == "crop")
				knob_intd_update(panel->get_knob("box"));
			else if (type == "position")
				knob_intd_update(panel->get_knob("translate"));
			else if (type == "cornel_pin")
			{
				knob_intd_update(panel->get_knob("to1"));
				knob_intd_update(panel->get_knob("to2"));
				knob_intd_update(panel->get_knob("to3"));
				knob_intd_update(panel->get_knob("to4"));
			}
		}
	};
	update();
}

void viewer_gl::knob_intd_update(knob *_knob)
{
	// actualiza desde el knob integer dimension hacia el manejador

	knob_intd *intd = static_cast <knob_intd*>(_knob);
	auto values = intd->get_values();
	QString type = intd->get_node_type();

	if (type == "crop")
		box_handler_update( intd->get_full_name(), { values[0], values[1], values[2], values[3] });
	else if (type == "position" || type == "cornel_pin")
		pos_handler_update( intd->get_full_name(), {values[0], values[1]});
}

void viewer_gl::draw_handlers()
{
	box_handlers_draw();
	pos_handler_draw();
}

knob *viewer_gl::get_knob(QString full_name)
{
	QStringList full = full_name.split(".");
	QString node_name = full[0];
	QString param_name = full[1];

	QWidget *_panel = static_cast<properties*>(_properties)->get_trim_panel(node_name);
	trim_panel *panel = static_cast<trim_panel *>(_panel);

	return panel->get_knob(param_name);
}

void viewer_gl::box_handler_changed(QString full_name, QRect box, bool release)
{
	// actualiza desde el manejador hacia el knob

	knob_intd *box_knob = static_cast<knob_intd*>(get_knob(full_name));
	box_knob->set_values({box.x(), box.y(), box.width(), box.height()}, false);
	if (release)
		box_knob->emmit_signal();
}

void viewer_gl::pos_handler_changed(QString full_name, QPoint position, bool release)
{
	knob_intd *translate = static_cast<knob_intd*>(get_knob(full_name));
	translate->set_values({position.x(), position.y()}, false);
	if (release)
		translate->emmit_signal();
}

