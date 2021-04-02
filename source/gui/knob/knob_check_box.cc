#include <knob_check_box.h>

knob_check_box::knob_check_box(QString _label, bool default_value)

    : label_widget(nullptr)
    , label(_label)
    , checked(default_value)
    , emmit_signal(true)
{
    this->setObjectName("knob_check_box");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    layout->addWidget(init_space);

    checkbox = new QCheckBox(this);
    checkbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(checkbox, &QCheckBox::stateChanged, this, [=](int state) {
        checked = state != 0;

        if (emmit_signal)
            changed(checked);

        // vulve al por defecto que es emitir señal
        emmit_signal = true;
    });
    set_check(checked);
    layout->addWidget(checkbox);

    if (!label.isEmpty())
    {
        label_widget = new QLabel(this);
        label_widget->setText(label);
        layout->addWidget(label_widget);
        layout->addStretch();
    }
}

knob_check_box::~knob_check_box() {}

void knob_check_box::set_check(bool value, bool _emmit_signal)
{
    emmit_signal = _emmit_signal;

    if (value == 0)
        checkbox->setCheckState(Qt::CheckState(0));
    else
        checkbox->setCheckState(Qt::CheckState(2));
}

bool knob_check_box::is_checked() const
{
    return checked;
}

void knob_check_box::mousePressEvent(QMouseEvent *event)
{
    set_check(!checked);
}
