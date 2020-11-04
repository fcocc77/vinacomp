#include <knob_check_box.hpp>

knob_check_box::knob_check_box(
    int _init_space_width,
    QString _label,
    bool _default_value)
{
    init_space_width = _init_space_width;
    label = _label;
    default_value = _default_value;

    QHBoxLayout *layout = new QHBoxLayout(this);

    init_space *_init_space = new init_space(init_space_width);
    layout->addWidget(_init_space);

    checkbox = new QCheckBox();
    set_check(default_value);
    layout->addWidget(checkbox);

    label_widget = new QLabel(label);
    layout->addWidget(label_widget);

    layout->addStretch();
}

knob_check_box::~knob_check_box()
{
}

void knob_check_box::set_check(bool value)
{
    if (value == 0)
        checkbox->setCheckState(Qt::CheckState(0));
    else
        checkbox->setCheckState(Qt::CheckState(2));
}