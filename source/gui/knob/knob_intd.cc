#include <knob_intd.h>

knob_intd::knob_intd(int dimensions_count, QList<int> defaults)
    : knob_dimensional(dimensions_count, int_to_float(defaults), false)
{
}

knob_intd::~knob_intd() {}

QList<float> knob_intd::int_to_float(QList<int> values) const
{
    QList<float> float_values;

    for (int item : values)
        float_values.push_back(item);

    return float_values;
}

QList<int> knob_intd::float_to_int(QList<float> values) const
{
    QList<int> int_values;

    for (float item : values)
        int_values.push_back(item);

    return int_values;
}

void knob_intd::changed_values(QList<float> values)
{
    QList<int> int_values;
    for (float value : values)
        int_values.push_back(value);

    changed(int_values);
}
