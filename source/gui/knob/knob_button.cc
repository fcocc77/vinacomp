#include <knob_button.h>

knob_button::knob_button(QString label)
{
	this->setObjectName("knob_button");

    QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	layout->setMargin(0);
	layout->addWidget(init_space);

    QPushButton *button = new QPushButton(label);
	connect(button, &QPushButton::clicked, this, [=](){
		clicked();
	});
    layout->addWidget(button);
}

knob_button::~knob_button()
{
}
