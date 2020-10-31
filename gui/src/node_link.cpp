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
    QBrush arrow_brush(Qt::black);
    arrow->setBrush(arrow_brush);
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
        QPen link_pen(Qt::white);
        link_pen.setWidth(6);
        this->setPen(link_pen);

        QBrush arrow_brush(Qt::white);
        QPen arrow_pen(Qt::white);
        arrow->setBrush(arrow_brush);
        arrow->setPen(arrow_pen);
    }
    else
    {
        QPen link_pen(Qt::black);
        link_pen.setWidth(4);
        this->setPen(link_pen);

        QBrush arrow_brush(Qt::black);
        QPen arrow_pen(Qt::black);
        arrow->setBrush(arrow_brush);
        arrow->setPen(arrow_pen);
    }
}

float node_link::arrow_refresh(QPointF point_a, QPointF point_b)
{
    float width = 7;
    float height = 25;
    int center_separation = 28;

    QPolygonF triangle;
    triangle.append(QPointF(-width, 0));
    triangle.append(QPointF(0., height));
    triangle.append(QPointF(width, 0));
    triangle.append(QPointF(-width, 0));

    arrow->setPolygon(triangle);

    // calcular la rotacion
    double delta_y = (point_a.y() - point_b.y());
    double delta_x = (point_b.x() - point_a.x());

    float rotation = atan2(delta_x, delta_y) * 180 / M_PI;
    arrow->setRotation(rotation);
    //

    // calcular la distancia entre el centro del nodo hasta el borde
    auto node_size = _node->get_size();
    float node_width_x = node_size.value(0) / 2;
    float node_width_y = node_size.value(1) / 2;

    float max_angle = atan2(node_width_x, node_width_y) * 180 / M_PI;

    float diagonal;

    int bottom_box = max_angle + (90 - max_angle) * 2;
    if (rotation < max_angle && rotation > -max_angle || rotation > bottom_box || rotation < -bottom_box)
    {
        float _width = (tan(rotation * M_PI / 180)) * node_width_y;
        diagonal = sqrt(pow(_width, 2) + pow(node_width_y, 2));
    }
    else
    {
        float _height = (tan((rotation + 90) * M_PI / 180)) * node_width_x;
        diagonal = sqrt(pow(_height, 2) + pow(node_width_x, 2));
    }
    //
    //

    arrow->setPos(point_a.x(), point_a.y() - diagonal - center_separation);
    arrow->setTransformOriginPoint(0, diagonal + center_separation);

    return diagonal;
}

void node_link::link_refresh(QPointF point_a, QPointF point_b)
{
    float diagonal = arrow_refresh(point_a, point_b);

    QLineF line = subtract_distance_line(QLineF(point_a, point_b), diagonal + 30);
    this->setLine(line);
}

QLineF node_link::subtract_distance_line(QLineF line, float distance)
{
    // le resta una distancia al una linea
    int distance_total = abs((line.x2() - line.x1()) + (line.y2() - line.y1()));

    float mag = sqrt(pow((line.x1() - line.x2()), 2) + pow((line.y1() - line.y2()), 2));
    float px = line.x1() - distance * (line.x1() - line.x2()) / mag;
    float py = line.y1() - distance * (line.y1() - line.y2()) / mag;

    return {line.p2(), {px, py}};
}

void node_link::update_connection()
{
    QPointF src_pos, dst_pos;

    src_pos = _node->get_center_position();
    if (connected_node != NULL)
        dst_pos = connected_node->get_center_position();
    else
        dst_pos = {src_pos.x(), src_pos.y() - link_size};

    link_refresh(src_pos, dst_pos);
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

    link_refresh(node_position, pos);

    *link_connecting = {{"name", _node->get_name()},
                        {"index", index}};
}
