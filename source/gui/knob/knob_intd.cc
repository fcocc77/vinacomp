#include <knob_intd.h>

knob_intd::knob_intd(QList<int> default_values)
// : knob_dimensional(int_to_float(default_values))
{

    // connect(changed, &knob_dimensional::changed, this,
    // [=](QList<float> values) {});
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
