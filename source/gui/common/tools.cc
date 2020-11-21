#include <tools.h>

tools::tools()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setObjectName("tools");

    layout = new QHBoxLayout(this);
    layout->setContentsMargins(15, 5, 15, 5);
    layout->setSpacing(2);
}

tools::~tools()
{
}

void tools::add_action(action *_action)
{
    QPushButton *button = new QPushButton();

    button->setToolTip(_action->get_label());
    qt::set_icon(button, _action->get_icon_name(), 22);

    connect(button, &QPushButton::clicked, this, [=]() {
        _action->trigger();
    });

    layout->addWidget(button);
}

void tools::add_separator()
{
    QWidget *vertical_separator = new QWidget();
    vertical_separator->setMinimumWidth(20);
    vertical_separator->setObjectName("separator");

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