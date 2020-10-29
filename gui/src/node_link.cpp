#include <node_link.hpp>

node_link::node_link(
    QGraphicsScene *_scene,
    node *__node)
{

    scene = _scene;
    _node = __node;

    QPen pen(Qt::black);
    pen.setWidth(25);

    this->setPen(pen);
    this->setLine(0, 0, 0, 50);

    scene->addItem(this);
}

node_link::~node_link()
{
}

void node_link::refresh()
{

    QPoint src_pos = _node->get_center_position();

    // if (connected_node != NULL)
    //     // dst_pos = connected_node->get_center_position();
    // else
    QPoint dst_pos = {src_pos.x(), src_pos.y() - 50};

    this->setLine(src_pos.x(), src_pos.y(), dst_pos.x(), dst_pos.y());
}

// Events:

void node_link::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void node_link::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // int x = node_center_position->x();
    // int y = node_center_position->y();

    // this->setLine(x, y, x, y - 50);

    // node *item = scene->itemAt(mapToScene(event->pos()), QTransform());
}

void node_link::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // QPointF pos = mapToScene(event->pos());

    // int x = node_center_position->x();
    // int y = node_center_position->y();

    // this->setLine(x, y, pos.x(), pos.y());
}
