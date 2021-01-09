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
	for (QPushButton *button : buttons)
		delete button;
}

void tools::add_action(action *_action)
{
    QPushButton *button = new QPushButton();
	buttons.push_back(button);

    button->setToolTip(_action->get_label());
	qt::set_icon(button, _action->get_icon_name() + "_a", icon_size);

    connect(button, &QPushButton::clicked, this, [=]() {
        _action->trigger();
		if (_action->is_checked())
			qt::set_icon(button, _action->get_icon_name() + "_c", icon_size);
		else
			qt::set_icon(button, _action->get_icon_name() + "_a", icon_size);

    });

    layout->addWidget(button);
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

