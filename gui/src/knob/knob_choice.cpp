#include <knob_choice.hpp>

knob_choice::knob_choice(/* args */)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(init_space);

    combo_box *choice = new combo_box();
    layout->addWidget(choice);

    layout->addStretch();
}

knob_choice::~knob_choice()
{
}