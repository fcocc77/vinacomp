#include <knob_floating.h>

knob_floating::knob_floating(float min, float max, float default_value,
                             bool two_dimensional)
    : knob_slider(min, max, default_value, two_dimensional)
{
    this->setObjectName("knob_floating");
}

knob_floating::~knob_floating() {}
