#include <knob_check_box.hpp>

knob_check_box::knob_check_box(/* args */)
{

    QHBoxLayout *layout = new QHBoxLayout(this);

    checkbox = new QCheckBox();
    layout->addWidget(checkbox);

    label = new QLabel("checkbox");
    layout->addWidget(label);

    layout->addStretch();
}

knob_check_box::~knob_check_box()
{
}