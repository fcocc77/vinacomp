#include <knob_floating.hpp>

knob_floating::knob_floating(
    QString _label,
    float _default_value)
{
    label = _label;
    default_value = _default_value;

    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *label_widget = new QLabel(label);
    layout->addWidget(label_widget);

    QLineEdit *value = new QLineEdit(QString::number(default_value));
    value->setMaximumWidth(50);
    layout->addWidget(value);

    slider *_slider = new slider();
    layout->addWidget(_slider);

    QPushButton *animation = new QPushButton();
    qt::set_icon(animation, "key_a", 20);
    layout->addWidget(animation);
}

knob_floating::~knob_floating()
{
}