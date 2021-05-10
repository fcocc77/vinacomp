#include <knob_floating.h>

knob_floating::knob_floating(knob_props props, float min, float max,
                             float default_value, bool bidimensional,
                             bool centered_handler)
    : knob_slider(props, min, max, default_value, bidimensional, true,
                  centered_handler)
{
    this->setObjectName("knob_floating");
}

knob_floating::~knob_floating() {}
