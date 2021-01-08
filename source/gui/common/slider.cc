#include <slider.h>

slider::slider(float _min, float _max, bool _floating)
	: floating(_floating),
	min(_min),
	max(_max),
	float_interval(100)
{
    this->setOrientation(Qt::Horizontal);

	if (floating)
	{
		this->setMinimum(min * float_interval);
		this->setMaximum(max * float_interval);
	}
	else
	{
		this->setMinimum(min);
		this->setMaximum(max);
	}

	connect(this, &QSlider::valueChanged, this, [=](int value){
		if(floating)
			moved(float( value ) / float_interval);
		else
			moved(value);
	});
}

slider::~slider()
{
}

void slider::set_value(float value)
{
	int _value = value * float_interval;
	this->setValue(_value);
}

void slider::mousePressEvent(QMouseEvent *event)
{
	// print("mover slider handler");
	QSlider::mousePressEvent(event);
}
