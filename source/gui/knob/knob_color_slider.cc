#include <knob_color_slider.h>
#include <color_picker.h>
#include <util.h>

knob_color_slider::knob_color_slider(float min, float max, float default_value,
                                     bool centered_handler, QString _label)
    : layout(new QHBoxLayout(this))
    , edit(new QLineEdit)
    , _slider(new slider(min, max, default_value, true, centered_handler))
    , label(new QLabel(_label))
    , _colored_slider(nullptr)
    , value(default_value)
{
    layout->setMargin(0);

    label->setFixedWidth(20);
    edit->setMaximumWidth(50);
    edit->setText(QString::number(default_value));

    // Conecciones
    connect(_slider, &slider::moved, this, [this](float _value) {
        value = _value;
        changed(value);
        edit->setText(QString::number(value));
    });

    connect(edit, &QLineEdit::editingFinished, this, [=]() {
        value = edit->text().toDouble();
        _slider->set_value(value);
        changed(value);
    });

    // Layout
    if (_label.isEmpty())
    {
        layout->addWidget(edit);
        layout->addWidget(_slider);
    }
    else
    {
        layout->addWidget(label);
        layout->addWidget(_slider);
        layout->addWidget(edit);
    }
}

knob_color_slider::~knob_color_slider()
{
    delete layout;
    delete edit;
    delete _colored_slider;
    delete _slider;
}

void knob_color_slider::set_hue_slider()
{
    _colored_slider = new colored_slider(_slider);
}

void knob_color_slider::set_value(float _value, bool emmit_signal)
{
    value = _value;
    _slider->set_value(value);
    edit->setText(QString::number(value));
}

void knob_color_slider::set_hue_color(float hue)
{
    if (!_colored_slider)
        return;

    _colored_slider->set_hue_color(hue);
}

void knob_color_slider::set_default_value(float _default_value)
{
    _slider->set_default_value(_default_value);
}

colored_slider::colored_slider(slider *__slider)
    : _slider(__slider)
{
    this->setObjectName("colored_slider");
    QHBoxLayout *layout = new QHBoxLayout(_slider->get_slider_center());
    layout->setContentsMargins(1, 5, 1, 0);
    this->setFixedHeight(1);

    layout->addWidget(this);
}

void colored_slider::set_hue_color(float hue)
{
    color = color_picker::hsl_to_rgb(hue);

    QString style = "background: rgb(%1, %2, %3);";

    _slider->get_handler()->setStyleSheet(
        style.arg(color.red()).arg(color.green()).arg(color.blue()));
}

void colored_slider::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QLinearGradient ramp(0, 0, width(), 0);
    float section = 1.0 / 6;

    ramp.setColorAt(0.0, Qt::red);
    ramp.setColorAt(section, Qt::yellow);
    ramp.setColorAt(section * 2, Qt::green);
    ramp.setColorAt(section * 3, Qt::cyan);
    ramp.setColorAt(section * 4, Qt::blue);
    ramp.setColorAt(section * 5, Qt::magenta);
    ramp.setColorAt(1.0, Qt::red);

    painter.fillRect(0, 0, width(), height(), QBrush(ramp));
}
