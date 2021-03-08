#include <viewer_gl.h>
#include <properties.h>
#include <trim_panel.h>

void viewer_gl::handlers_update()
{
	panels = static_cast< properties * >(_properties)->get_trim_panels();

	box_handler_clear();
	pos_handler_clear();
	tf_handler_clear();

	for (QWidget *_panel : panels)
	{
		trim_panel *panel = static_cast<trim_panel *>(_panel);
		if (panel->maximized())
		{
			QString type = panel->get_type();

			if (type == "crop")
				knob_signal(panel->get_knob("box"));
			else if (type == "position")
				knob_signal(panel->get_knob("translate"));
			else if (type == "cornel_pin")
			{
				knob_signal(panel->get_knob("to1"));
				knob_signal(panel->get_knob("to2"));
				knob_signal(panel->get_knob("to3"));
				knob_signal(panel->get_knob("to4"));
			}
			else if (type == "transform")
			{
				knob_signal(panel->get_knob("rotate"));
			}
		}
	};

	udpate();
}

void viewer_gl::knob_signal(knob *_knob)
{
	// actualiza desde el knob hacia el manejador
	QString param_name = _knob->get_name();
	QString node_name = _knob->get_node_name();
	QString type = _knob->get_node_type();
	QString name = _knob->get_full_name();

	knob_floating *floating = dynamic_cast<knob_floating*>(_knob);
	knob_intd *intd = dynamic_cast <knob_intd*>(_knob);
	knob_floatd *floatd = dynamic_cast <knob_floatd*>(_knob);

	if (type == "transform")
	{
		if (param_name == "rotate" && floating)
			tf_handler_rotate_update(node_name, floating->get_value());
		else if (param_name == "translate" && floatd)
		{
			auto values = floatd->get_values();
			tf_handler_translate_update(node_name, {values[0], values[1]});
		}
	}

	else if (type == "crop" && intd)
	{
		auto values = intd->get_values();
		box_handler_update(name, type, { values[0], values[1], values[2], values[3] });
	}
	else if (type == "position" && intd)
	{
		auto values = intd->get_values();
		pos_handler_update(name, type, {values[0], values[1]});
	}
	else if (type == "cornel_pin" && intd)
	{
		auto values = intd->get_values();
		pos_handler_update(name, type, {values[0], values[1]});
	}

	update();
}

void viewer_gl::draw_handlers()
{
	box_handlers_draw();
	pos_handler_draw();
	tf_handler_draw();
}

knob *viewer_gl::get_knob(QString node_name, QString param_name)
{
	QWidget *_panel = static_cast<properties*>(_properties)->get_trim_panel(node_name);
	trim_panel *panel = static_cast<trim_panel *>(_panel);

	return panel->get_knob(param_name);
}

void viewer_gl::box_handler_changed(QString name, QString type, QRect box, bool release)
{
	// actualiza desde el manejador hacia el knob
	QString node_name = name.split(".")[0];
	QString param_name = name.split(".")[1];

	if (type == "crop")
	{
		knob_intd *box_knob = static_cast<knob_intd*>(get_knob(node_name, param_name));
		box_knob->set_values({box.x(), box.y(), box.width(), box.height()}, false);
		if (release)
			box_knob->emmit_signal();
	}
}

void viewer_gl::pos_handler_changed(pos_handler_struct handler, bool release)
{
	QString type = handler.type;
	QString node_name = handler.name.split(".")[0];
	QString param_name = handler.name.split(".")[1];

	if (type == "position" || type == "cornel_pin")
	{
		knob_intd *translate = static_cast<knob_intd*>(get_knob(node_name, param_name));
		translate->set_values({handler.position.x(), handler.position.y()}, false);
		if (release)
			translate->emmit_signal();
	}
}

void viewer_gl::tf_handler_changed(tf_handler_struct handler, bool release)
{
	knob_floating *rotate_knob = static_cast<knob_floating*>(get_knob(handler.name, "rotate"));
	knob_floatd *translate_knob = static_cast<knob_floatd*>(get_knob(handler.name, "translate"));

	rotate_knob->set_value(handler.rotate, release);
	translate_knob->set_values({handler.translate.x(), handler.translate.y()}, release);
}

