#include <knob_integer.hpp>

knob_integer::knob_integer(/* args */)
{

    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *label = new QLabel("INteger");
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

knob_integer::~knob_integer()
{
}