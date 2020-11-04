#include <knob_floating.hpp>

knob_floating::knob_floating(
    int _init_space_width,
    QString _label,
    float _default_value)
{
    init_space_width = _init_space_width;
    label = _label;
    default_value = _default_value;

    QHBoxLayout *layout = new QHBoxLayout(this);

    init_space *label_widget = new init_space(init_space_width, label);
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