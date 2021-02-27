#include <viewer_gl.h>
#include <properties.h>
#include <trim_panel.h>

void viewer_gl::handlers_update()
{
	panels = dynamic_cast< properties * >(_properties)->get_trim_panels();
	update();
}

void viewer_gl::draw_handlers()
{
	for (QWidget *_panel : panels)
	{
		trim_panel *panel = dynamic_cast<trim_panel *>(_panel);
		if (panel->get_type() == "crop")
		{
			knob_intd *box_knob = dynamic_cast <knob_intd*>(panel->get_knob("box"));
			auto values = box_knob->get_values();
			// box_handler_draw({ values[0], values[1], values[2], values[3] });
		};
	};
}

void viewer_gl::box_handler_changed(QRect box, QString name)
{
}

