#include <frame_range_panel.h>
#include <knob_intd.h>

frame_range_panel::frame_range_panel() {}

frame_range_panel::~frame_range_panel() {}

void frame_range_panel::changed(knob *_knob)
{
    QString param_name = _knob->get_name();

    if (param_name == "reset")
    {
        knob_intd *frame_range_knob = static_cast<knob_intd *>(get_knob("frame_range"));
        // auto frame_range = _renderer->get_frame_range(_renderer->get_input_node(name));
        // frame_range_knob->set_values({frame_range.first, frame_range.second});
    }
}
