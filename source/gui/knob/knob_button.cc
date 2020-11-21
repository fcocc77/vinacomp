#include <knob_button.h>

knob_button::knob_button(/* args */)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QPushButton *button = new QPushButton("BUTTON");
    layout->addWidget(button);

    layout->addStretch();
}

knob_button::~knob_button()
{
}