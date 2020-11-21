#include <knob_integer.h>

knob_integer::knob_integer(/* args */)
{

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(init_space);

    QLineEdit *value = new QLineEdit();
    value->setMaximumWidth(50);
    layout->addWidget(value);

    slider *_slider = new slider();
    layout->addWidget(_slider);

    QPushButton *animation = new QPushButton();
    qt::set_icon(animation, "key_a", 20);
    layout->addWidget(animation);
}

knob_integer::~knob_integer()
{
}