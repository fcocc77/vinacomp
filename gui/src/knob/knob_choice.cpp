#include <knob_choice.hpp>

knob_choice::knob_choice(/* args */)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *label = new QLabel("Choice");
    layout->addWidget(label);

    combo_box *choice = new combo_box();
    layout->addWidget(choice);

    layout->addStretch();
}

knob_choice::~knob_choice()
{
}