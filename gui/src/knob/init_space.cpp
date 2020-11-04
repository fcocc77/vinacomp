#include <init_space.hpp>

init_space::init_space(int width, QString label)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label_widget = new QLabel(label);
    layout->addStretch();
    layout->addWidget(label_widget);
    layout->setMargin(0);

    this->setMaximumWidth(width);
    this->setMinimumWidth(width);

    this->setObjectName("init_space");
}

init_space::~init_space()
{
}