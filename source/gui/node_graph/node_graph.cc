#include <node_graph.h>

node_graph::node_graph(
    QJsonObject *_project,
    properties *_properties)
{

    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    nodes_loaded = new nodes_load();

    _node_view = new node_view(_project, _properties);
    maker *_maker = new maker(_properties, nodes_loaded, _node_view);
    _nodes_bar = new nodes_bar(_maker, nodes_loaded);

    layout->addWidget(_nodes_bar);
    layout->addWidget(_node_view);
}

node_graph::~node_graph()
{
}
