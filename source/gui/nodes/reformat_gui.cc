#include <reformat_gui.h>
#include <knob_choice.h>

reformat_gui::reformat_gui() {}

reformat_gui::~reformat_gui() {}

void reformat_gui::changed(knob *_knob)
{
    QString param_name = _knob->get_name();

    if (param_name == "type")
    {
        knob_choice *type = static_cast<knob_choice *>(_knob);
        int index = type->get_index();

        knob *format_knob = get_knob("format");
        knob *scale_knob = get_knob("scale");
        knob *custom_knob = get_knob("custom");

        if (index == 0)
        {
            format_knob->set_visible(true);
            scale_knob->set_visible(false);
            custom_knob->set_visible(false);
        }
        else if (index == 1)
        {
            format_knob->set_visible(false);
            scale_knob->set_visible(true);
            custom_knob->set_visible(false);
        }
        else if (index == 2)
        {
            format_knob->set_visible(false);
            scale_knob->set_visible(false);
            custom_knob->set_visible(true);
        }
    }
}
