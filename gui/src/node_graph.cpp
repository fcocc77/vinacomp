#include <node_graph.hpp>

node_graph::node_graph(/* args */)
{

    QGraphicsView *graphics_view = new QGraphicsView();
    QHBoxLayout *layout = new QHBoxLayout();

    layout->addWidget(graphics_view);
    this->setLayout(layout);
}

node_graph::~node_graph()
{
}