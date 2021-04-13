#include <knob_integer.h>

knob_integer::knob_integer(int min, int max, int default_value,
                           bool two_dimensional)
    : knob_slider(min, max, default_value, two_dimensional, false)
{
    this->setObjectName("knob_integer");
}

knob_integer::~knob_integer() {}

