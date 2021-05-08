#include <knob_floatd.h>

knob_floatd::knob_floatd(knob_props props, int dimensions_count,
                         QList<float> default_values)
    : knob_dimensional(props, dimensions_count, default_values)
{
}

knob_floatd::~knob_floatd() {}

void knob_floatd::changed_values(QList<float> values)
{
    changed(values);
}
