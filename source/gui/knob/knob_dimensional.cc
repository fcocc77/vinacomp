#include <knob_dimensional.h>

knob_dimensional::knob_dimensional(knob_props props, int _dimensions_count,
                                   QList<float> _default_values, bool _floating)
    : knob(props)
    , default_values(_default_values)
    , floating(_floating)
    , dimensions_count(_dimensions_count)
{
    this->setObjectName("knob_dimensions");
    QHBoxLayout *layout = new QHBoxLayout(this);
    this->set_knob_layout(layout);
    layout->setMargin(0);

    layout->addWidget(init_space);

    for (int i = 0; i < dimensions_count; i++)
    {
        float value = default_values.value(i);
        QLineEdit *dimension_edit = new QLineEdit(QString::number(value));

        connect(dimension_edit, &QLineEdit::editingFinished, this, [=]() {
            values.clear();
            for (int i = 0; i < dimensions_count; i++)
                values.push_back(get_value(i));

            emmit_signal();
            update_handler();
        });

        dimension_edit->setMaximumWidth(50);
        layout->addWidget(dimension_edit);
        dimensions_edits.push_back(dimension_edit);
    }

    layout->addStretch();
}

knob_dimensional::~knob_dimensional()
{
    for (QLineEdit *edit : dimensions_edits)
        delete edit;
}

void knob_dimensional::restore_default()
{
    knob::restore_default();
    set_values(default_values, false);
}

void knob_dimensional::restore_param()
{
    knob::restore_param();

    QJsonArray values = get_param_value().toArray();

    QList<float> default_dimensions, dimensions;
    for (QJsonValue value : values)
        default_dimensions.push_back(value.toDouble());

    set_values(default_dimensions, false);
}

void knob_dimensional::emmit_signal()
{
    changed_values(values); // Signal

    QJsonArray output_values;
    for (float value : values)
    {
        if (floating)
            output_values.push_back(value);
        else
            output_values.push_back(int(value));
    }

    update_value(output_values);
}

void knob_dimensional::changed_values(QList<float> values) {}

float knob_dimensional::get_value(int dimension) const
{
    if (dimension >= dimensions_edits.count())
        return 0;

    return dimensions_edits.value(dimension)->text().toInt();
}

QList<float> knob_dimensional::get_values() const
{
    QList<float> values;
    for (int i = 0; i < dimensions_edits.count(); i++)
        values.push_back(dimensions_edits.value(i)->text().toInt());

    return values;
}

void knob_dimensional::set_value(float value, int dimension)
{
    if (dimension >= dimensions_edits.count())
        return;

    dimensions_edits.value(dimension)->setText(QString::number(value));
}

void knob_dimensional::set_values(QList<float> _values, bool _emmit_signal)
{
    values = _values;

    for (int i = 0; i < dimensions_edits.count(); i++)
        set_value(values.value(i), i);

    if (_emmit_signal)
        emmit_signal();
}
