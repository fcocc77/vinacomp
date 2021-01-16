#include <knob_floating.h>

knob_floating::knob_floating(float min, float max, float default_value)
{
    this->setObjectName("knob_floating");
	layout = new QHBoxLayout(this);
	layout->setMargin(0);
    layout->addWidget(init_space);
	this->set_knob_layout(layout);

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
		changed(value); // Signal
	});

    layout->addWidget(_slider);

	set_value(default_value);
}

knob_floating::~knob_floating()
{
	delete value_edit;
	delete _slider;
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
