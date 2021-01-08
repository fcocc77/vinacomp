#include <knob_position.h>

knob_position::knob_position(/* args */)
{
	this->setObjectName("knob_position");
    QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);

    layout->addWidget(init_space);

    QLineEdit *value_x = new QLineEdit();
    value_x->setMaximumWidth(50);
    layout->addWidget(value_x);

    QLineEdit *value_y = new QLineEdit();
    value_y->setMaximumWidth(50);
    layout->addWidget(value_y);

    layout->addStretch();

    QPushButton *animation = new QPushButton();
    qt::set_icon(animation, "key_a", icon_size);
    layout->addWidget(animation);
}

knob_position::~knob_position()
{
}
