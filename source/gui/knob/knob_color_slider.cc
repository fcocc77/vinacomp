#include <knob_color_slider.h>

knob_color_slider::knob_color_slider(float min, float max, float default_value,
                                     bool centered_handler)
    : layout(new QHBoxLayout(this))
    , edit(new QLineEdit)
    , _slider(new slider(min, max, default_value, true, centered_handler))
{
    layout->setMargin(0);

    edit->setMaximumWidth(50);
    edit->setText(QString::number(default_value));

    // Conecciones
    connect(_slider, &slider::moved, this, [this](float value) {
        changed(value);
        edit->setText(QString::number(value));
    });

    connect(edit, &QLineEdit::editingFinished, this, [=]() {
        float value = edit->text().toDouble();
        _slider->set_value(value);
        changed(value);
    });

    // Layout
    layout->addWidget(edit);
    layout->addWidget(_slider);
}

knob_color_slider::~knob_color_slider()
{
    delete layout;
    delete edit;
    delete _slider;
}

void knob_color_slider::set_value(float value, bool emmit_signal ) {}

void knob_color_slider::set_default_value(float _default_value)
{
    _slider->set_default_value(_default_value);
}
