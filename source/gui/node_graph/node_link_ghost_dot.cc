#include <link.h>
#include <maker.h>
#include <node.h>
#include <node_graph.h>
#include <node_link_ghost_dot.h>
#include <node_view.h>
#include <util.h>

ghost_dot::ghost_dot(int size, QWidget *__node_graph, QGraphicsItem *_link)
    : QGraphicsEllipseItem(0, 0, size, size)
    , _node_graph(__node_graph)
    , link(_link)
{
    this->setBrush(Qt::green);
}

ghost_dot::~ghost_dot() {}

void ghost_dot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    node_graph *graph = static_cast<node_graph *>(_node_graph);

    node_view *__node_view = graph->get_node_view();
    maker *_maker = graph->get_maker();
    node_link *_link = static_cast<node_link *>(link);

    node *_connected_node = static_cast<node *>(_link->get_connected_node());

    __node_view->set_visible_ghost_dots(false);
    QString dot_name = _maker->create_fx("dot");

    node *dot = __node_view->get_node(dot_name);

    _link->disconnect_node();
    _link->connect_node(dot);

    dot->get_link(1)->connect_node(_connected_node);
    dot->grabMouse();
}
