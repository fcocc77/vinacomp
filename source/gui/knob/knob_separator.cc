#include <knob_separator.h>

knob_separator::knob_separator(/* args */)
{
    this->setObjectName("knob_separator");
    this->setMinimumHeight(25);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(init_space);
    layout->setMargin(0);
    QWidget *line = new QWidget(this);
    line->setMaximumHeight(1);
    layout->addWidget(line);
}

knob_separator::~knob_separator() {}
