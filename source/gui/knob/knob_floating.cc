#include <knob_floating.h>

knob_floating::knob_floating(
    float _default_value)
	: default_value(_default_value),
	min(0),
	max(5)
{
    this->setObjectName("knob_floating");
	layout = new QHBoxLayout(this);
	layout->setMargin(0);
    layout->addWidget(init_space);

    value_edit = new QLineEdit();
	connect(value_edit, &QLineEdit::returnPressed, this, [=](){
		float value = value_edit->text().toDouble();
		set_value(value);
	});
    value_edit->setMaximumWidth(50);
    layout->addWidget(value_edit);

	_slider = new slider(min, max);
	connect(_slider, &slider::moved, this, [=](float value){
		value_edit->setText(QString::number(value));
	});

    layout->addWidget(_slider);

    animation_button = new QPushButton(this);
    qt::set_icon(animation_button, "key_a", 20);
    layout->addWidget(animation_button);

	set_value(default_value);
}

knob_floating::~knob_floating()
{
	delete value_edit;
	delete _slider;
	delete animation_button;
	delete layout;
}

void knob_floating::set_value(float value)
{
	value_edit->setText(QString::number(value));
	_slider->set_value(value);
}

float knob_floating::get_value() const
{
	return value_edit->text().toDouble();
}
