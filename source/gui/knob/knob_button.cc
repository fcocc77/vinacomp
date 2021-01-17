#include <knob_button.h>

knob_button::knob_button(QString label)
{
	this->setObjectName("knob_button");

    QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	layout->setMargin(0);
	layout->addWidget(init_space);

    QPushButton *button = new QPushButton(label);
	// button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	// button->setMinimumWidth(100);
	// button->setMaximumWidth(100);
    layout->addWidget(button);
}

knob_button::~knob_button()
{
}
