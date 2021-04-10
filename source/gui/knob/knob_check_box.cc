#include <knob_check_box.h>

knob_check_box::knob_check_box(QString label, bool default_value)

    : label_widget(nullptr)
    , checked(default_value)
    , emmit_signal(true)
{
    this->setObjectName("knob_check_box");

    layout = new QHBoxLayout(this);
    layout->setMargin(0);

    layout->addWidget(init_space);

    checkbox = new QCheckBox(this);
    checkbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(checkbox, &QCheckBox::stateChanged, this, [=](int state) {
        checked = state != 0;

        if (emmit_signal)
        {
            changed(checked);
            update_value(checked);
        }

        // vulve al por defecto que es emitir señal
        emmit_signal = true;
    });
    set_check(checked);
    layout->addWidget(checkbox);

    if (!label.isEmpty())
        set_label(label);
}

knob_check_box::~knob_check_box() {}

void knob_check_box::restore_param()
{
    QJsonValue param_value = get_param_value();
    bool value;

    // if (animated)
        // value = anim::get_value(param_value.toString(), project->frame);
    // else
    value = param_value.toBool();

    set_check(value);
    set_label(get_label());
}

void knob_check_box::set_label(QString label)
{
    if (!label_widget)
    {
        label_widget = new QLabel(this);
        layout->addWidget(label_widget);
        layout->addStretch();
    }

    label_widget->setText(label);
}

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
