#include <knob_color_slider.h>

knob_color_slider::knob_color_slider(float min, float max, float default_value,
                                     bool centered_handler, QString _label)
    : layout(new QHBoxLayout(this))
    , edit(new QLineEdit)
    , _slider(new slider(min, max, default_value, true, centered_handler))
    , label(new QLabel(_label))
    , value(default_value)
{
    layout->setMargin(0);

    label->setFixedWidth(20);
    edit->setMaximumWidth(50);
    edit->setText(QString::number(default_value));

    // Conecciones
    connect(_slider, &slider::moved, this, [this](float _value) {
        value = _value;
        changed(value);
        edit->setText(QString::number(value));
    });

    connect(edit, &QLineEdit::editingFinished, this, [=]() {
        value = edit->text().toDouble();
        _slider->set_value(value);
        changed(value);
    });

    // Layout
    if (_label.isEmpty())
    {
        layout->addWidget(edit);
        layout->addWidget(_slider);
    }
    else
    {
        layout->addWidget(label);
        layout->addWidget(_slider);
        layout->addWidget(edit);
    }
}

knob_color_slider::~knob_color_slider()
{
    delete layout;
    delete edit;
    delete _slider;
}

void knob_color_slider::set_value(float _value, bool emmit_signal)
{
    value = _value;
    _slider->set_value(value);
    edit->setText(QString::number(value));
}

void knob_color_slider::set_default_value(float _default_value)
{
    _slider->set_default_value(_default_value);
}
