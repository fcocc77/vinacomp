#include <node_graph.hpp>

node_graph::node_graph(/* args */)
{
    scene = new QGraphicsScene();

    this->setObjectName("node_graph");
    this->setScene(scene);

    // desabilita el scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    //

    nodes = new QList<node *>;

    scene->setSceneRect(-500000, -500000, 1000000, 1000000);

    panning = false;
    pressed = false;

    add_node("Transform", 100, 0);
    add_node("Grade", 300, 0);

    this->setRenderHint(QPainter::Antialiasing);
}

node_graph::~node_graph()
{
}

void node_graph::add_node(QString name, int x, int y)
{

    node *_node = new node(scene);
    _node->set_name(name);
    _node->set_position(x, y);

    nodes->push_back(_node);
}
