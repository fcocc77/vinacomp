#include <maker.h>
#include <node_graph.h>
#include <node_link_ghost_dot.h>
#include <node_view.h>
#include <util.h>

ghost_dot::ghost_dot(int size, QWidget *__node_graph)
    : QGraphicsEllipseItem(0, 0, size, size)
    , _node_graph(__node_graph)
{
    this->setBrush(Qt::green);
}

ghost_dot::~ghost_dot() {}

void ghost_dot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    node_graph *graph = static_cast<node_graph *>(_node_graph);
    node_view *__node_view = graph->get_node_view();

    maker *_maker = graph->get_maker();

    __node_view->set_visible_ghost_dots(false);
    QString dot_name = _maker->create_fx("dot");
}
