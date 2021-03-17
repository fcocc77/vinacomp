#include <knob_label.h>

knob_label::knob_label(QString label)
{
    this->setObjectName("knob_label");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(init_space);
    int margin = 5;
    layout->setContentsMargins(0, margin, 0, margin);

    QLabel *label_widget = new QLabel(this);
    label_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    label_widget->setText(label);

    QWidget *start_line = new QWidget(this);
    QWidget *end_line = new QWidget(this);
    start_line->setMaximumHeight(1);
    end_line->setMaximumHeight(1);

    layout->addWidget(start_line);
    layout->addWidget(label_widget);
    layout->addWidget(end_line);
}

knob_label::~knob_label() {}
