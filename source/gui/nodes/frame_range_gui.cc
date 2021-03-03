#include <frame_range_gui.h>

frame_range_gui::frame_range_gui(){}

frame_range_gui::~frame_range_gui(){}

void frame_range_gui::changed(QString param_name)
{
	if (param_name == "reset")
	{
		knob_intd *frame_range_knob = static_cast<knob_intd*>(get_knob("frame_range"));
		auto frame_range = _renderer->get_frame_range(_renderer->get_input_node(name));
		frame_range_knob->set_values({frame_range.first, frame_range.second});
	}
}

