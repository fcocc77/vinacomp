#include <knob_progress.h>

knob_progress::knob_progress(knob_props props)
    : knob(props)
{
    this->setObjectName("knob_progress");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    layout->setMargin(0);
    layout->addWidget(init_space);

    bar = new QProgressBar();
    layout->addWidget(bar);
}

knob_progress::~knob_progress() {}

void knob_progress::restore_param() {}
