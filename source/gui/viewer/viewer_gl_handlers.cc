#include <viewer_gl.h>
#include <properties.h>
#include <trim_panel.h>

void viewer_gl::handlers_update()
{
	panels = dynamic_cast< properties * >(_properties)->get_trim_panels();

	clear_box_handler();
	for (QWidget *_panel : panels)
	{
		trim_panel *panel = dynamic_cast<trim_panel *>(_panel);
		if (panel->get_type() == "crop")
		{
			knob_intd *box_knob = dynamic_cast <knob_intd*>(panel->get_knob("box"));
			auto values = box_knob->get_values();
			add_box_handler(
				panel->get_name(),
				{ values[0], values[1], values[2], values[3] }
			);
		};
	};
	update();
}

void viewer_gl::draw_handlers()
{
	box_handlers_draw();
}

knob *viewer_gl::get_knob(QString panel_name, QString knob_name)
{
	QWidget *_panel = dynamic_cast<properties*>(_properties)->get_trim_panel(panel_name);
	trim_panel *panel = dynamic_cast<trim_panel *>(_panel);

	return panel->get_knob(knob_name);
}

void viewer_gl::box_handler_changed(QRect box, QString name)
{
}

void viewer_gl::box_handler_finished(QRect box, QString name)
{
	knob_intd *box_knob = dynamic_cast<knob_intd*>(get_knob(name, "box"));
	box_knob->set_value({box.x(), box.y(), box.width(), box.height()});
}

