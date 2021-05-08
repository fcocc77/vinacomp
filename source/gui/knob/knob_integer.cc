#include <knob_integer.h>

knob_integer::knob_integer(knob_props props, int min, int max,
                           int default_value, bool two_dimensional,
                           bool centered_handler)
    : knob_slider(props, min, max, default_value, two_dimensional, false,
                  centered_handler)
{
    this->setObjectName("knob_integer");
}

knob_integer::~knob_integer() {}

