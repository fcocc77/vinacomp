#include <knob_intd.h>

knob_intd::knob_intd(int dimensions_count, QList<int> defaults)
    : knob_dimensional(dimensions_count, int_to_float(defaults))
{
}

knob_intd::~knob_intd() {}

QList<float> knob_intd::int_to_float(QList<int> defaults)
{
    QList<float> float_defaults;

    for (int item : defaults)
        float_defaults.push_back(item);

    return float_defaults;
}

void knob_intd::changed_values(QList<float> values)
{
    QList<int> int_values;
    for (float value : values)
        int_values.push_back(value);

    changed(int_values);
}
