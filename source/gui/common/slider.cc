#include <slider.h>
#include <util.h>

slider::slider(float min, float max, float default_value, bool _floating)
    : floating(_floating)
    , float_interval(100)
    , emmit_signal(true)
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
        if (emmit_signal)
        {
            if (floating)
                moved(float(value) / float_interval);
            else
                moved(value);
        }
    });

    this->setValue(default_value);
}

slider::~slider() {}

void slider::set_value(float value, bool _emmit_signal)
{
    emmit_signal = _emmit_signal;

    if (floating)
        value *= float_interval;

    this->setValue(value);
}

void slider::mousePressEvent(QMouseEvent *event)
{
    // print("mover slider handler");
    QSlider::mousePressEvent(event);
}
