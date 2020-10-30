#include <node_link.hpp>

node_link::node_link(
    int _index,
    QGraphicsScene *_scene,
    node *__node,
    QJsonObject *_link_connecting)
{

    scene = _scene;
    _node = __node;
    link_connecting = _link_connecting;
    connected_node = NULL;
    index = _index;

    QPen pen(Qt::black);
    pen.setWidth(5);

    this->setPen(pen);
    this->setLine(0, 0, 0, 50);

    scene->addItem(this);

    refresh();
}

node_link::~node_link()
{
}

void node_link::refresh()
{
    update_connection();
}

void node_link::update_connection()
{
    QPoint src_pos, dst_pos;

    src_pos = _node->get_center_position();
    if (connected_node != NULL)
        dst_pos = connected_node->get_center_position();
    else
        dst_pos = {src_pos.x(), src_pos.y() - 50};

    this->setLine(src_pos.x(), src_pos.y(), dst_pos.x(), dst_pos.y());
}

void node_link::connect_node(node *_node)
{
    if (_node == NULL)
        return;

    connected_node = _node;
    update_connection();
}

void node_link::disconnect_node()
{
    if (connected_node)
        connected_node->remove_output_node(_node);

    connected_node = NULL;
    update_connection();
}

// Events:
void node_link::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void node_link::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update_connection();
}

void node_link::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
    QPoint node_position = _node->get_center_position();

    int x = node_position.x();
    int y = node_position.y();

    this->setLine(x, y, pos.x(), pos.y());

    *link_connecting = {{"name", _node->get_name()},
                        {"index", index}};
}
