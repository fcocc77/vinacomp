#include "../node_graph/node.h"
#include <math.h>
#include <node_link.h>
#include <util.h>
#include <vinacomp.h>

node_link::node_link(QString input_label, bool _has_mask, int _index,
                     QGraphicsScene *_scene, QGraphicsItem *__node,
                     QJsonObject *_link_connecting, project_struct *_project,
                     QWidget *__vinacomp, QWidget *_node_graph)

    : scene(_scene)
    , this_node(__node)
    , connected_node(nullptr)
    , link_connecting(_link_connecting)
    , project(_project)
    , _vinacomp(__vinacomp)
    , link_size(70)
    , index(_index)
    , dragging(false)
    , visible(true)
    , label(input_label)
    , has_mask(_has_mask)
    , ghost_dot_visible(false)
    , ghost_dot_size(20)
{

    // Link
    link = new QGraphicsLineItem();
    QPen pen(Qt::black);
    pen.setWidth(2);
    if (index == 0) // mask
        pen.setStyle(Qt::DashLine);
    link->setPen(pen);
    link->setLine(0, 0, 0, link_size);
    scene->addItem(link);
    //
    //

    // Links fantasmas, para la insercion de nodos
    ghost_link_a = new QGraphicsLineItem();
    ghost_link_b = new QGraphicsLineItem();
    QPen ghost_link_pen(Qt::green);
    ghost_link_pen.setStyle(Qt::DashLine);
    ghost_link_a->setPen(ghost_link_pen);
    ghost_link_b->setPen(ghost_link_pen);
    ghost_link_a->setLine(0, 0, 0, link_size);
    ghost_link_b->setLine(0, 0, 0, link_size);
    scene->addItem(ghost_link_a);
    scene->addItem(ghost_link_b);
    set_ghost_link(false);
    //
    //

    // Ajuste del rectangulo, el rectangulo es esta misma clase
    // y es transparente, por que es solo para que el agarre
    // de la flecha funcione mejor.
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
    text->setPlainText(label);
    text->setDefaultTextColor(QColor(200, 200, 200));
    scene->addItem(text);
    //
    //

    // Ghost Dot
    _ghost_dot = new ghost_dot(ghost_dot_size, _node_graph, this);
    scene->addItem(_ghost_dot);
    //

    // nombre para identificar que es un link
    this->setData(0, "link");
    link->setData(0, "link");
    arrow->setData(0, "link");
    text->setData(0, "link");
    //

    refresh();
}

node_link::~node_link() {}

void node_link::set_ghost_link(bool visible, QPointF break_point)
{
    ghost_link_a->setVisible(visible);
    ghost_link_b->setVisible(visible);

    if (!connected_node || !visible)
        return;

    node *_this_node = static_cast<node *>(this_node);
    node *_connected_node = static_cast<node *>(connected_node);

    QPointF src_pos = _this_node->get_center_position();
    QPointF dst_pos = _connected_node->get_center_position();

    ghost_link_a->setLine({src_pos, break_point});
    ghost_link_b->setLine({dst_pos, break_point});
}

void node_link::update_visibility()
{
    // establece visibilidad tomando en cuenta
    // cada link conectado y la cantidad de indexs

    // si el link es el 0 que mascara, y el nodo no tiene mascara, no sera
    // visible
    if (index == 0 && !has_mask)
    {
        set_visible(false);
        return;
    }

    node *_this_node = static_cast<node *>(this_node);
    auto connected = _this_node->get_connected_indexs();

    if (connected[index] || index == 1 || index == 2)
    {
        // si el link es el 1 o 2 o si este link esta conectado, sera visible
        set_visible(true);
        return;
    }
    else if (index == 0) // mask
    {
        bool visible = _this_node->is_selected();
        set_visible(visible);
        return;
    }

    // verifica si todos los links superiores a 2 e inferiores al index actual
    // esten conectados, si estan todos conectados, el link actual sera visible
    bool visible = true;
    for (int i = 2; i < index; i++)
    {
        if (!connected[i])
        {
            visible = false;
            break;
        }
    }

    set_visible(visible);
}

QLineF node_link::get_line_from_node() const
{
    node *_this_node = static_cast<node *>(this_node);
    QPointF src_pos, dst_pos;

    src_pos = _this_node->get_center_position();
    if (connected_node)
    {
        node *_connected_node = static_cast<node *>(connected_node);
        dst_pos = _connected_node->get_center_position();
    }
    else
    {
        int width = _this_node->get_size().width() / 2;

        if (index == 0) // index 0 es 'mask'
            dst_pos = {src_pos.x() + width - 20 + link_size, src_pos.y()};
        else if (index == 1)
            dst_pos = {src_pos.x(), src_pos.y() - link_size};
        else
            dst_pos = {src_pos.x() - width + 20 - link_size, src_pos.y()};
    }

    return {src_pos, dst_pos};
}

void node_link::refresh()
{
    QLineF line = get_line_from_node();
    update_visibility();
    link_refresh(line.p1(), line.p2());
}

void node_link::insert_node_in_between(QGraphicsItem *_node)
{
    if (!connected_node)
        return;

    QGraphicsItem *_connected_node = connected_node;
    node *between_node = static_cast<node *>(_node);

    if (_node == _connected_node || _node == this_node)
        return;

    this->disconnect_node();
    this->connect_node(between_node);

    between_node->get_link()->connect_node(_connected_node);
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

QPointF node_link::get_center(QPointF point_a, QPointF point_b) const
{
    float x = (point_a.x() + point_b.x()) / 2;
    float y = (point_a.y() + point_b.y()) / 2;

    return {x, y};
}

void node_link::set_selected(bool enable)
{
    if (enable)
    {
        QPen link_pen(Qt::white);
        if (index == 0) // mask
            link_pen.setStyle(Qt::DashLine);

        link_pen.setWidth(4);
        link->setPen(link_pen);

        QBrush arrow_brush(Qt::white);
        QPen arrow_pen(Qt::white);
        arrow->setBrush(arrow_brush);
        arrow->setPen(arrow_pen);
    }
    else
    {
        QPen link_pen(Qt::black);
        if (index == 0) // mask
            link_pen.setStyle(Qt::DashLine);
        link_pen.setWidth(2);
        link->setPen(link_pen);

        QBrush arrow_brush(Qt::black);
        QPen arrow_pen(Qt::black);
        arrow->setBrush(arrow_brush);
        arrow->setPen(arrow_pen);
    }

    refresh();
}

void node_link::text_refresh(QPointF point_a, QPointF point_b)
{
    if (!visible)
    {
        text->hide();
        return;
    }

    QPointF center = get_center(point_a, point_b);

    int text_width = text->boundingRect().width();
    int text_height = text->boundingRect().height();

    if (dragging || connected_node)
    {
        text->setPos(center.x() - (text_width / 2),
                     center.y() - (text_height / 2));

        if (dragging)
        {
            text->show();
        }
        else
        {
            if (get_long(point_a, point_b) > (text_width + 250))
                text->show();
            else
                text->hide();
        }
    }
    else
    {
        text->setPos(point_b.x() - (text_width / 2), point_b.y() - text_height);
        text->show();
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

    float rotation = get_rotation(point_a, point_b);
    arrow->setRotation(rotation);
    //

    // calcular la distancia entre el centro del nodo hasta el borde
    auto node_size = static_cast<node *>(this_node)->get_size();
    float node_width_x = node_size.width() / 2;
    float node_width_y = node_size.height() / 2;

    float max_angle = atan2(node_width_x, node_width_y) * 180 / M_PI;

    float diagonal;

    int bottom_box = max_angle + (90 - max_angle) * 2;
    if (rotation < max_angle && rotation > -max_angle ||
        rotation > bottom_box || rotation < -bottom_box)
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

    QLineF line =
        subtract_distance_line(QLineF(point_a, point_b), diagonal + 20);
    link->setLine(line);

    bbox_refresh(point_a, point_b);
    text_refresh(point_a, point_b);

    // Ghost Dot
    if (!ghost_dot_visible || !connected_node)
    {
        _ghost_dot->setVisible(false);
        return;
    }

    text->setVisible(false);

    int mid_diameter = ghost_dot_size / 2;
    QPointF center = get_center(point_a, point_b);
    _ghost_dot->setPos({center.x() - mid_diameter, center.y() - mid_diameter});

    _ghost_dot->setVisible(true);
}

QLineF node_link::subtract_distance_line(QLineF line, float distance)
{
    // le resta una distancia a una linea
    float mag =
        sqrt(pow((line.x1() - line.x2()), 2) + pow((line.y1() - line.y2()), 2));

    float px = line.x1() - distance * (line.x1() - line.x2()) / mag;
    float py = line.y1() - distance * (line.y1() - line.y2()) / mag;

    return {line.p2(), {px, py}};
}

bool node_link::safe_connection(QGraphicsItem *node_to_connect) const
{
    // previene que el link a conectar vuelva al mismo nodo
    // y provoque un bucle infinito
    node *_this_node = static_cast<node *>(this_node);
    node *_to_node = static_cast<node *>(node_to_connect);

    // evita que se conecte asi mismo
    if (_this_node == _to_node)
        return false;

    for (node *_node : *_to_node->get_input_nodes())
    {
        if (!safe_connection(_node))
            return false;
    }

    return true;
}

void node_link::connect_node(QGraphicsItem *to_node, bool prevent_loop)
{
    if (!to_node)
        return;

    node *_to_node = static_cast<node *>(to_node);

    if (_to_node->get_type() == "backdrop")
        return;

    node *_this_node = static_cast<node *>(this_node);

    // evita que se provoque un bucle infinito
    if (prevent_loop)
        if (!safe_connection(_to_node))
            return;

    if (connected_node)
    {
        node *_connected_node = static_cast<node *>(connected_node);
        _connected_node->remove_output_node(_this_node);
        _connected_node->refresh();
    }

    _to_node->add_output_node(_this_node);

    _this_node->add_input_node(_to_node);
    _this_node->set_connected_index(index, true);

    connected_node = _to_node;

    // añade la entrada al proyecto y actualiza el render
    project->insert_input(_this_node->get_name(), _to_node->get_name(), index);
    static_cast<vinacomp *>(_vinacomp)->update_render_all_viewer(true);
    //

    dragging = false;
    _to_node->refresh();
    _this_node->refresh();
}

void node_link::disconnect_node()
{
    if (!connected_node)
        return;

    node *_connected_node = static_cast<node *>(connected_node);
    node *_this_node = static_cast<node *>(this_node);

    _connected_node->remove_output_node(_this_node);

    _this_node->remove_input_node(_connected_node);
    _this_node->set_connected_index(index, false);

    // borra la entrada del proyecto y actualiza el render
    project->delete_input(_this_node->get_name(), index);
    static_cast<vinacomp *>(_vinacomp)->update_render_all_viewer(true);
    //

    connected_node = nullptr;
    dragging = false;

    _connected_node->refresh();
    _this_node->refresh();
}

void node_link::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // tiene que existir el 'mousePressEvent' para que funcione el
    // 'mouseMoveEvent'
}

void node_link::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    dragging = false;
    refresh();
}

void node_link::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    node *_this_node = static_cast<node *>(this_node);
    dragging = true;

    QPointF pos = mapToScene(event->pos());
    QPointF node_position = _this_node->get_center_position();

    link_refresh(node_position, pos);

    *link_connecting = {{"name", _this_node->get_name()}, {"index", index}};
}
