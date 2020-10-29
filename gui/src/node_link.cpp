#include <node.hpp>

node_link::node_link(QPoint *_node_center_position)
{

    node_center_position = _node_center_position;

    QPen pen(Qt::black);
    pen.setWidth(25);

    this->setPen(pen);
    this->setLine(0, 0, 100, 50);
}

node_link::~node_link()
{
}

void node_link::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void node_link::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    int x = node_center_position->x();
    int y = node_center_position->y();

    this->setLine(x, y, x, y - 50);
}

void node_link::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());

    int x = node_center_position->x();
    int y = node_center_position->y();

    this->setLine(x, y, pos.x(), pos.y());
}