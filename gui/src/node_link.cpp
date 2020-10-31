#include <node_link.hpp>

node_link::node_link(
    int _index,
    QGraphicsScene *_scene,
    node *__node,
    QJsonObject *_link_connecting)
{

    scene = _scene;
    this_node = __node;
    link_connecting = _link_connecting;
    connected_node = NULL;
    index = _index;

    // Link
    link_size = 70;
    link = new QGraphicsLineItem();
    QPen pen(Qt::black);
    pen.setWidth(4);
    link->setPen(pen);
    link->setLine(0, 0, 0, link_size);
    scene->addItem(link);
    //
    //

    // Ajuste del rectangulo, el rectangulo es esta misma clase
    // y es transparente, por que es solo
    // para que el agarre de la flecha funcione mejor.
    QPen rectangle_pen(Qt::transparent);
    rectangle_pen.setWidth(1);
    this->setPen(rectangle_pen);
    scene->addItem(this);
    //
    //

    // Flecha
    arrow = new QGraphicsPolygonItem();
    QBrush arrow_brush(Qt::black);
    arrow->setBrush(arrow_brush);
    scene->addItem(arrow);
    //
    //

    //  Texto
    text = new QGraphicsTextItem();
    QFont font;
    font.setPointSize(10);
    text->setFont(font);
    text->setPlainText("Input");
    text->setDefaultTextColor(QColor(200, 200, 200));
    scene->addItem(text);
    //
    //

    // nombre para identificar que es un link
    this->setData(0, "link");
    link->setData(0, "link");
    arrow->setData(0, "link");
    //

    refresh();
}

node_link::~node_link()
{
}

void node_link::refresh()
{
    set_selected(this_node->is_selected());
    update_connection();
}

float node_link::get_rotation(QPointF point_a, QPointF point_b)
{
    // calcular la rotacion a partir de 2 puntos
    double delta_y = (point_a.y() - point_b.y());
    double delta_x = (point_b.x() - point_a.x());

    return atan2(delta_x, delta_y) * 180 / M_PI;
}

float node_link::get_long(QPointF point_a, QPointF point_b)
{
    float x = pow(point_b.x() - point_a.x(), 2);
    float y = pow(point_b.y() - point_a.y(), 2);

    return sqrt(x + y);
}

QPointF node_link::get_center(QPointF point_a, QPointF point_b)
{
    float x = (point_a.x() + point_b.x()) / 2;
    float y = (point_a.y() + point_b.y()) / 2;

    return {x, y};
}

void node_link::set_selected(bool enable)
{
    if (this_node->is_selected())
    {
        QPen link_pen(Qt::white);
        link_pen.setWidth(5);
        link->setPen(link_pen);

        QBrush arrow_brush(Qt::white);
        QPen arrow_pen(Qt::white);
        arrow->setBrush(arrow_brush);
        arrow->setPen(arrow_pen);
    }
    else
    {
        QPen link_pen(Qt::black);
        link_pen.setWidth(2);
        link->setPen(link_pen);

        QBrush arrow_brush(Qt::black);
        QPen arrow_pen(Qt::black);
        arrow->setBrush(arrow_brush);
        arrow->setPen(arrow_pen);
    }
}

void node_link::text_refresh(QPointF point_a, QPointF point_b)
{
    QPointF center = get_center(point_a, point_b);

    int text_width = text->boundingRect().width();
    int text_height = text->boundingRect().height();

    if (dragging || connected_node)
        text->setPos(center.x() - (text_width / 2), center.y() - (text_height / 2));
    else
        text->setPos(point_b.x() - (text_width / 2), point_b.y() - text_height);
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

    float rotation = get_rotation(point_a, point_b);
    arrow->setRotation(rotation);
    //

    // calcular la distancia entre el centro del nodo hasta el borde
    auto node_size = this_node->get_size();
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

void node_link::bbox_refresh(QPointF point_a, QPointF point_b)
{
    int width = 40;
    int height = get_long(point_a, point_b);

    QRect rect(-width / 2, -height, width, height);

    this->setRect(rect);
    this->setPos(point_a.x(), point_a.y());

    float rotation = get_rotation(point_a, point_b);
    this->setRotation(rotation);
}

void node_link::link_refresh(QPointF point_a, QPointF point_b)
{
    float diagonal = arrow_refresh(point_a, point_b);

    QLineF line = subtract_distance_line(QLineF(point_a, point_b), diagonal + 20);
    link->setLine(line);

    bbox_refresh(point_a, point_b);
    text_refresh(point_a, point_b);
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

    src_pos = this_node->get_center_position();
    if (connected_node != NULL)
        dst_pos = connected_node->get_center_position();
    else
        dst_pos = {src_pos.x(), src_pos.y() - link_size};

    link_refresh(src_pos, dst_pos);
}

void node_link::connect_node(node *to_node)
{
    if (to_node == NULL)
        return;

    if (connected_node)
        connected_node->remove_output_node(this_node);

    to_node->add_output_node(this_node);

    connected_node = to_node;
    update_connection();
}

void node_link::disconnect_node()
{
    if (connected_node)
        connected_node->remove_output_node(this_node);

    connected_node = NULL;
    update_connection();
}

// Events:
void node_link::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void node_link::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    dragging = false;
    update_connection();
}

void node_link::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    dragging = true;

    QPointF pos = mapToScene(event->pos());
    QPointF node_position = this_node->get_center_position();

    float x = node_position.x();
    float y = node_position.y();

    link_refresh(node_position, pos);

    *link_connecting = {{"name", this_node->get_name()},
                        {"index", index}};
}
