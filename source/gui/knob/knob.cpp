#include <knob.hpp>

knob::knob(/* args */)
{

    // Espacio inicial
    init_space = new QWidget();
    init_space->setObjectName("init_space");

    QHBoxLayout *layout = new QHBoxLayout(init_space);
    label_widget = new QLabel();
    layout->addStretch();
    layout->addWidget(label_widget);
    layout->setMargin(0);
    //
    //
}

knob::~knob()
{
}

void knob::set_init_space(int space, QString label)
{
    if (space == 0)
        init_space->hide();
    else
    {
        init_space->setMaximumWidth(space);
        init_space->setMinimumWidth(space);
        label_widget->setText(label);
    }
}