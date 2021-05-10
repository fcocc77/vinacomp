#include <knob_button.h>

knob_button::knob_button(knob_props props)
    : knob(props)
{
    this->setObjectName("knob_button");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    layout->setMargin(0);
    layout->addWidget(init_space);

    button = new QPushButton();
    button->setToolTip(get_tips());
    connect(button, &QPushButton::clicked, this, [=]() {
        // el boton solo funciona en gui
        to_node_gui(this);
    });
    layout->addWidget(button);
}

knob_button::~knob_button() {}

void knob_button::restore_param()
{
    button->setText(get_label());
}
