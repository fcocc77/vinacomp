#include <knob_group.h>

knob_group::knob_group(QString label, int _knobs_included)
	: knobs_included(_knobs_included),
	visible_knobs(true)
{
	this->setObjectName("knob_group");
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 10, 0, 0);
	layout->setSpacing(0);
    layout->addWidget(init_space);

	label_widget = new QLabel(this);
	label_widget->setText(label);

	layout->addWidget(label_widget);

	arrow_button = new QPushButton(this);
	connect(arrow_button, &QPushButton::clicked, this, [=](){
		toggle_open();
	});
    arrow_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	qt::set_icon(arrow_button, "arrow_down_a", 18);
	init_space->layout()->addWidget(arrow_button);
}

knob_group::~knob_group()
{
}

void knob_group::toggle_open()
{
	visible_knobs = !visible_knobs;
	for (QWidget *knob : get_knobs_included())
		knob->setVisible(visible_knobs);

	label_widget->setProperty("open", visible_knobs);
	label_widget->style()->unpolish(label_widget);
   	label_widget->style()->polish(label_widget);

	init_space->setProperty("open", visible_knobs);
	init_space->style()->unpolish(init_space);
   	init_space->style()->polish(init_space);

	// cambio de icono de flecha
	if (visible_knobs)
		qt::set_icon(arrow_button, "arrow_down_a", 18);
	else
		qt::set_icon(arrow_button, "arrow_right_a", 18);
}

QList <QWidget *> knob_group::get_knobs_included()
{
	// obtiene todos los 'knobs' incluidos en este grupo,
	// el metodo usado es obtener el padre, e iterar todos los hijos,
	// al encontrar este mismo grupo se suman los widget siguientes.

	QWidget *parent = this->parentWidget();
	bool this_group = false;

	QList<QWidget *> knobs;
	int knobs_added = 0;
    for (int i = 0; i < parent->layout()->count(); i++)
    {
        QWidget *widget = parent->layout()->itemAt(i)->widget();
		
		if (this_group)
		{
			if (knobs_added >= knobs_included)
				break;
			knobs.push_back(widget);
			knobs_added ++;
		}

		if (widget == this)
			this_group = true;
    }

	return knobs;
}

void knob_group::mousePressEvent(QMouseEvent *event)
{
	toggle_open();
}
