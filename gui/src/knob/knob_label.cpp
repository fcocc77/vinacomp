#include <knob_label.hpp>

knob_label::knob_label(/* args */)
{

    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *label = new QLabel("LABEL:");
    layout->addWidget(label);

    layout->addStretch();
}

knob_label::~knob_label()
{
}