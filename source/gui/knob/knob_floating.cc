#include <knob_floating.h>

knob_floating::knob_floating(
    float _default_value)
{

    default_value = _default_value;

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(init_space);

    value_edit = new QLineEdit(QString::number(default_value));
    value_edit->setMaximumWidth(50);
    layout->addWidget(value_edit);

	_slider = new slider(-5, 5);
	connect(_slider, &slider::moved, this, [=](float value){
		value_edit->setText(QString::number(value));
	});

    layout->addWidget(_slider);

    QPushButton *animation = new QPushButton();
    qt::set_icon(animation, "key_a", 20);
    layout->addWidget(animation);

	set_value(-3.4);
}

knob_floating::~knob_floating()
{
}

void knob_floating::set_value(float value)
{
	value_edit->setText(QString::number(value));
	_slider->set_value(value);
}
