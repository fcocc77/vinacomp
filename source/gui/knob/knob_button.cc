#include <knob_button.h>

knob_button::knob_button()
{
    this->setObjectName("knob_button");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    layout->setMargin(0);
    layout->addWidget(init_space);

    button = new QPushButton();
    connect(button, &QPushButton::clicked, this, [=]() {
        // el boton solo funciona en gui
        changed(get_name());
    });
    layout->addWidget(button);
}

knob_button::~knob_button() {}

void knob_button::restore_param()
{
    button->setText(get_label());
}
