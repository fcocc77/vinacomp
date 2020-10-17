#include <node.hpp>

node::node(/* args */)
{

    QLabel *label = new QLabel("Nodo");
    QHBoxLayout *layout = new QHBoxLayout();

    layout->addWidget(label);

    this->setLayout(layout);
}

node::~node()
{
}
