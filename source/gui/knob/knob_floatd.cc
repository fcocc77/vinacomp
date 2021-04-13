#include <knob_floatd.h>

knob_floatd::knob_floatd(QList<float> default_values)
    : knob_dimensional(default_values)
{
}

knob_floatd::~knob_floatd() {}

void knob_floatd::changed_values(QList<float> values)
{
    changed(values);
}
