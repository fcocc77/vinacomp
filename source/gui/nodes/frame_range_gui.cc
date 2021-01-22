#include <frame_range_gui.h>

frame_range_gui::frame_range_gui(){}

frame_range_gui::~frame_range_gui(){}

void frame_range_gui::clicked()
{
	knob_dimensions *frame_range = dynamic_cast<knob_dimensions*>(get_knob("frame_range"));
	// modificarla con renderer ...
}

