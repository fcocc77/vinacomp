#include <slider.h>

slider::slider(float min, float max, float default_value, bool _floating)
    : floating(_floating)
    , float_interval(100)
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

    connect(this, &QSlider::valueChanged, this, [=](int value) {
        if (floating)
            moved(float(value) / float_interval);
        else
            moved(value);
    });

    this->setValue(default_value);
}

slider::~slider() {}

void slider::set_value(float value)
{
    if (floating)
        value *= float_interval;

    this->setValue(value);
}

void slider::mousePressEvent(QMouseEvent *event)
{
    // print("mover slider handler");
    QSlider::mousePressEvent(event);
}
