#include <knob.h>

knob::knob(/* args */)
	: animation_button(nullptr)
	, knob_layout(nullptr)
{
    // Espacio inicial
    init_space = new QWidget();
	init_space->hide();
	init_space->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    init_space->setObjectName("init_space");

    QHBoxLayout *layout = new QHBoxLayout(init_space);
    label_widget = new QLabel();
    layout->addStretch();
    layout->addWidget(label_widget);
    layout->setMargin(0);
    //
    //
	
	icon_size = 15;
}

knob::~knob()
{
	delete animation_button;
}

void knob::set_init_space(int space, QString label)
{
    if (space == 0)
		return;

	init_space->show();
	init_space->setMaximumWidth(space);
	init_space->setMinimumWidth(space);
	label_widget->setText(label);
}

void knob::set_knob_layout(QHBoxLayout *layout)
{
	// es el layout del hijo que heredo esta clase, para poder agregarle 
	// el boton de animacion final.
	knob_layout = layout;
}

void knob::set_animatable(bool _animatable)
{
	if (animation_button || !knob_layout)
		return;

	animation_button = new QPushButton();
	animation_button->setObjectName("small_button");
	qt::set_icon(animation_button, "key_a", icon_size);
	knob_layout->addWidget(animation_button);
}
