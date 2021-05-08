#include <knob_label.h>

knob_label::knob_label()
{
    this->setObjectName("knob_label");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(init_space);
    int margin = 5;
    layout->setContentsMargins(0, margin, 0, margin);

    label_widget = new QLabel(this);
    label_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QWidget *start_line = new QWidget(this);
    QWidget *end_line = new QWidget(this);
    start_line->setObjectName("start_line");
    end_line->setObjectName("end_line");
    start_line->setMaximumHeight(1);
    end_line->setMaximumHeight(1);

    layout->addWidget(start_line);
    layout->addWidget(label_widget);
    layout->addWidget(end_line);
}

knob_label::~knob_label() {}

void knob_label::restore_param()
{
    knob::restore_param();
    label_widget->setText(get_label());
}
