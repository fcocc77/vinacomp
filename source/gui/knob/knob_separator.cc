#include <knob_separator.h>

knob_separator::knob_separator(knob_props props)
    : knob(props)
{
    this->setObjectName("knob_separator");
    this->setFixedHeight(25);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(init_space);
    layout->setMargin(0);
    QWidget *line = new QWidget(this);
    line->setObjectName("knob_separator_line");
    line->setMaximumHeight(1);
    layout->addWidget(line);
}

knob_separator::~knob_separator() {}
