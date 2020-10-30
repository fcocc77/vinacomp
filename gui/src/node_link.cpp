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

    link_size = 70;

    QPen pen(Qt::black);
    pen.setWidth(4);

    this->setPen(pen);
    this->setLine(0, 0, 0, link_size);

    scene->addItem(this);
    //

    // Flecha
    arrow = new QGraphicsPolygonItem();
    arrow->setParentItem(this);
    //
    //

    // nombre para identificar que es un link
    this->setData(0, "link");
    arrow->setData(0, "link");
    //

    refresh();
}

node_link::~node_link()
{
}

void node_link::refresh()
{
    set_selected(_node->is_selected());
    update_connection();
}

void node_link::set_selected(bool enable)
{
    if (_node->is_selected())
    {
        QPen pen(Qt::white);
        pen.setWidth(6);
        this->setPen(pen);
    }
    else
    {
        QPen pen(Qt::black);
        pen.setWidth(4);
        this->setPen(pen);
    }
}

void node_link::arrow_refresh(QPointF point_a, QPointF point_b)
{
    float width = 10;
    float height = 30;

    QPolygonF triangle;
    triangle.append(QPointF(-width, 0));
    triangle.append(QPointF(0., height));
    triangle.append(QPointF(width, 0));
    triangle.append(QPointF(-width, 0));

    arrow->setPolygon(triangle);

    QBrush arrow_brush(Qt::black);
    arrow->setBrush(arrow_brush);

    // calcular la rotacion
    double delta_y = (point_a.y() - point_b.y());
    double delta_x = (point_b.x() - point_a.x());

    float rotation = atan2(delta_x, delta_y) * 180 / M_PI;
    arrow->setRotation(rotation);
    //

    // calcular la distancia entre el centro del nodo hasta el border
    auto node_size = _node->get_size();
    float node_width_x = node_size.value(0) / 2;
    float node_width_y = node_size.value(1) / 2;

    float max_angle = atan2(node_width_x, node_width_y) * 180 / M_PI;

    // float distance_form_angle = (node_width_y + ((diagonal - node_width_y) * (rotation / max_angle)));
    //

    node_width_x *= rotation / max_angle;
    int diagonal = sqrt(pow(node_width_x, 2) + pow(node_width_y, 2));

    arrow->setPos(point_a.x(), point_a.y() - diagonal - 30);
    arrow->setTransformOriginPoint(0, diagonal + 30);
}

void node_link::update_connection()
{
    QPointF src_pos, dst_pos;

    src_pos = _node->get_center_position();
    if (connected_node != NULL)
        dst_pos = connected_node->get_center_position();
    else
        dst_pos = {src_pos.x(), src_pos.y() - link_size};

    this->setLine(src_pos.x(), src_pos.y(), dst_pos.x(), dst_pos.y());

    arrow_refresh(src_pos, dst_pos);
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
    QPointF node_position = _node->get_center_position();

    float x = node_position.x();
    float y = node_position.y();

    this->setLine(x, y, pos.x(), pos.y());
    arrow_refresh(node_position, pos);

    *link_connecting = {{"name", _node->get_name()},
                        {"index", index}};
}
