#include <knob_choice.h>

knob_choice::knob_choice(QStringList items, int default_index)
{
	this->setObjectName("knob_choice");
    QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);

    layout->addWidget(init_space);

    choice = new combo_box(items, default_index);
	connect(choice, &combo_box::changed, this, &knob_choice::changed);

    layout->addWidget(choice);
    layout->addStretch();
}

knob_choice::~knob_choice()
{
}
