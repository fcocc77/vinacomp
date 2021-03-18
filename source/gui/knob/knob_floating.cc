#include <knob_floating.h>

knob_floating::knob_floating(float min, float max, float default_value, int _dimensions_amount)
    : emmit_signal(true)
    , dimensions(false)
    , dimensions_amount(_dimensions_amount)
    , show_dimensions(nullptr)

{
    this->setObjectName("knob_floating");
    layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(init_space);
    this->set_knob_layout(layout);

    value_edit = new QLineEdit();
    connect(value_edit, &QLineEdit::editingFinished, this, [=]() {
        float value = value_edit->text().toDouble();
        set_value(value);
    });
    value_edit->setMaximumWidth(50);
    layout->addWidget(value_edit);

    _slider = new slider(min, max);
    connect(_slider, &slider::moved, this, [=](float value) {
        value_edit->setText(QString::number(value));
        if (emmit_signal)
        {
            update_handler();
            changed(value); // Signal
        }
    });

    layout->addWidget(_slider);

    if (dimensions_amount > 1)
    {
        show_dimensions = new button();
        show_dimensions->setText(QString::number(dimensions_amount));
        connect(show_dimensions, &QPushButton::clicked, this, [=]() {
            dimensions = !dimensions;
            qt::set_property(show_dimensions, "active", dimensions);
        });

        layout->addWidget(show_dimensions);
    }

    set_value(default_value);
}

knob_floating::~knob_floating()
{
    delete value_edit;
    delete _slider;
    delete layout;
}

void knob_floating::set_value(float value, bool _emmit_signal)
{
    emmit_signal = _emmit_signal;

    value_edit->setText(QString::number(value));
    _slider->set_value(value);
}

float knob_floating::get_value() const
{
    return value_edit->text().toDouble();
}
