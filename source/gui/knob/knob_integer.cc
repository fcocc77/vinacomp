#include <knob_integer.h>

knob_integer::knob_integer(/* args */)
{
	this->setObjectName("knob_integer");
    QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);

    layout->addWidget(init_space);

    value_edit = new QLineEdit(this);
    value_edit ->setMaximumWidth(50);
    layout->addWidget(value_edit);

    slider *_slider = new slider(0, 10, false);
	connect(_slider, &slider::moved, this, [=](int value){
		value_edit->setText(QString::number(value));
	});
    layout->addWidget(_slider);

    QPushButton *animation = new QPushButton();
    qt::set_icon(animation, "key_a", 20);
    layout->addWidget(animation);
}

knob_integer::~knob_integer()
{
}
