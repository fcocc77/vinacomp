#include <slider.h>
#include <util.h>

slider::slider(float _min, float _max, float _default_value, bool _floating)
    : floating(_floating)
    , float_interval(100)
    , handler_percent(50)
    , min(_min)
    , max(_max)
    , default_value(_default_value)
    , center_default_value(true)
{
    // QVBoxLayout *layout = new QVBoxLayout;
    // layout->setMargin(0);
    // this->setLayout(layout);

    setObjectName("slider");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    slider_center = new QWidget(this);
    slider_center->move(0, 6);

    slider_center->setObjectName("slider_center");
    slider_center->setMaximumHeight(6);

    handler = new QWidget(this);
    int handler_size = 15;
    handler->setMaximumSize({handler_size, handler_size});
    handler->setObjectName("slider_handler");

    // layout->addWidget(slider);
    // layout->addWidget(handler);

    set_min_max(min, max);

    // connect(this, &QSlider::valueChanged, this, [=](int value) {
    // if (emmit_signal)
    // {
    // if (floating)
    // moved(float(value) / float_interval);
    // else
    // moved(value);
    // }
    // });

    // this->setValue(default_value);
}

slider::~slider() {}

void slider::set_min_max(float min, float max)
{
    if (floating)
    {
        // this->setMinimum(min * float_interval);
        // this->setMaximum(max * float_interval);
    }
    else
    {
        // this->setMinimum(min);
        // this->setMaximum(max);
    }
}

float slider::get_percent_by_value(float value)
{
    if (!center_default_value)
        return (value - min) * 100.0 / (max - min);

    // transforma manualmente el valor al porcentaje para mantener el 'handler'
    // en el centro, dividiendo en 4 partes la slider, no es esta la mejor
    // forma, pero, la mejor forma es revertir lo hecho en
    // 'get_value_by_percent' pero es muy dificil hacerlo

    auto get_percent = [=](float _min, float _max) {
        float percent = (value - _min) * 100.0 / (_max - _min);
        if (percent <= 100 && percent >= 0)
            return percent;
        else
            return (float)-1;
    };

    float default_fraction = (default_value - min) / (max - min);

    float m1 = min;
    float x1 = min + ((default_value - min) * default_fraction);
    float p1 = get_percent(m1, x1);

    float m2 = x1;
    float x2 = default_value;
    float p2 = get_percent(m2, x2);

    float m3 = default_value;
    float x3 = default_value + ((max - default_value) * default_fraction);
    float p3 = get_percent(m3, x3);

    float m4 = x3;
    float x4 = max;
    float p4 = get_percent(m4, x4);

    float result = 0;

    if (p1 != -1)
    {
        result = p1 / 4;
    }
    else if (p2 != -1)
    {
        result = p2 / 4;
        result += 25;
    }
    else if (p3 != -1)
    {
        result = p3 / 4;
        result += 50;
    }
    else if (p4 != -1)
    {
        result = p4 / 4;
        result += 75;
    }

    if (value < min)
        result = 0;
    else if (value > max)
        result = 100;

    return result;
}

float slider::get_value_by_percent(float percent)
{
    if (!center_default_value)
        return (((max - min) * handler_percent) / 100.0) + min;

    // hace un calculo para que siempre el 'handler' este en el centro,
    // a partir de 'default_value', usamos para esto el algoritmo de bezier

    float default_fraction = (default_value - min) / (max - min);

    // parte 1 va de 'min' a 'default_value'
    float value = (percent * 2.0) / 100.0;

    float p1_min = min;
    float p1_max = default_value;
    float p1_mid = p1_min + ((p1_max - p1_min) * default_fraction);

    float p1_a = ((1 - value) * p1_min) + (value * p1_mid);
    float p1_b = ((1 - value) * p1_mid) + (value * p1_max);
    float p1_value = ((1 - value) * p1_a) + (p1_b * value);
    //

    // parte 2 va de 'default_value' a 'max'
    value = (percent - 50) / 50.0;

    float p2_min = default_value;
    float p2_max = max;
    float p2_mid = p2_min + ((p2_max - p2_min) * default_fraction);

    float p2_a = ((1 - value) * p2_min) + (value * p2_mid);
    float p2_b = ((1 - value) * p2_mid) + (value * p2_max);
    float p2_value = ((1 - value) * p2_a) + (p2_b * value);
    //

    float result = p1_value;
    if (percent > 50)
        result = p2_value;

    return result;
}

void slider::set_value(float value, bool emmit_signal)
{
    handler_percent = get_percent_by_value(value);
    refresh();

    // if (emmit_signal)
    // to_emmit_signal();
}

void slider::to_emmit_signal()
{
    float value = get_value_by_percent(handler_percent);

    // limita los decimales a 2
    float value_round = int(value * 1000.0) / 1000.0;

    moved(value_round);
}

void slider::refresh()
{
    if (handler_percent > 100)
        handler_percent = 100;
    else if (handler_percent < 0)
        handler_percent = 0;

    int pos_x = (width() - handler->width()) * handler_percent / 100;
    handler->move({pos_x, 0});
}

void slider::resizeEvent(QResizeEvent *event)
{
    slider_center->setFixedWidth(event->size().width());
    refresh();
}

void slider::mousePressEvent(QMouseEvent *event)
{
    handler_percent = event->x() * 100.0 / (float)width();
    refresh();
    to_emmit_signal();
}

void slider::mouseMoveEvent(QMouseEvent *event)
{
    handler_percent = event->x() * 100.0 / (float)width();
    refresh();
    to_emmit_signal();
}
