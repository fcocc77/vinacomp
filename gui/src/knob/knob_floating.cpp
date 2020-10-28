#include <knob_floating.hpp>

knob_floating::knob_floating(/* args */)
{

    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *label = new QLabel("Floating");
    layout->addWidget(label);

    QLineEdit *value = new QLineEdit();
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