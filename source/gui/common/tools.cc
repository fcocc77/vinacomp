#include <tools.h>

tools::tools(int _icon_size)
	: icon_size(_icon_size)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setObjectName("tools");

    layout = new QHBoxLayout(this);
    layout->setContentsMargins(15, 5, 15, 5);
    layout->setSpacing(2);
}

tools::~tools()
{
	for (action *_action : actions)
		delete _action;
}

void tools::add_action(action *_action)
{
	actions.push_back(_action);

	// permite que solo se pueda hacer 1 solo 'check' a la vez
	bool uncheck_all = true;
	//
	QPushButton *button = _action->make_button(this, icon_size, uncheck_all);
    layout->addWidget(button);
}

void tools::set_checked_all(bool checked)
{
	for (action *_action : actions)
		_action->set_checked(checked);
}

void tools::add_separator()
{
    QWidget *vertical_separator = new QWidget();
    vertical_separator->setMinimumWidth(20);
    vertical_separator->setObjectName("tools_separator");

    layout->addWidget(vertical_separator);
}

void tools::add_stretch()
{
    layout->addStretch();
}

void tools::add_widget(QWidget *widget)
{
    layout->addWidget(widget);
}

QHBoxLayout *tools::get_layout() const
{
	return layout;
}

