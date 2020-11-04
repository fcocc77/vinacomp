#include <knob_check_box.hpp>

knob_check_box::knob_check_box(
    QString _label,
    bool _default_value)
{
    label = _label;
    default_value = _default_value;

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(init_space);

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