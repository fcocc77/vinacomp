#include <knob_floating.hpp>

knob_floating::knob_floating(
    float _default_value)
{

    default_value = _default_value;

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(init_space);

    QLineEdit *value = new QLineEdit(QString::number(default_value));
    value->setMaximumWidth(50);
    layout->addWidget(value);

    slider *_slider = new slider();
    layout->addWidget(_slider);

    QPushButton *animation = new QPushButton();
    qt::set_icon(animation, "key_a", 20);
    layout->addWidget(animation);
}

knob_floating::~knob_floating()
{
}