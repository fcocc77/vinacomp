#include "node.h"
#include <node_link.h>
#include <node_output_link.h>
#include <node_view.h>
#include <util.h>

output_link::output_link(QGraphicsScene *_scene, QWidget *__node_view,
                         QGraphicsItem *_this_node)
    : this_node(_this_node)
    , scene(_scene)
    , _node_view(__node_view)
{

    QPen rectangle_pen(Qt::transparent);
    rectangle_pen.setWidth(1);
    this->setPen(rectangle_pen);

    scene->addItem(this);

    // Link
    link = new QGraphicsLineItem();
    QPen pen(Qt::black);
    pen.setWidth(2);
    link->setPen(pen);
    scene->addItem(link);
    //
    //

    // Flecha
    arrow = new QGraphicsPolygonItem();
    QBrush arrow_brush(Qt::black);
    arrow->setBrush(arrow_brush);
    scene->addItem(arrow);
    //

    this->setData(0, "link");
    arrow->setData(0, "link");

    refresh();
}

output_link::~output_link() {}

void output_link::set_selected(bool enable)
{
    if (enable)
    {
        arrow->setPen(QPen(Qt::white));
        arrow->setBrush(QBrush(Qt::white));
        link->setPen(QPen(Qt::white, 4));
    }
    else
    {
        arrow->setPen(QPen(Qt::black));
        arrow->setBrush(QBrush(Qt::black));
        link->setPen(QPen(Qt::black, 2));
    }
}

void output_link::refresh_arrow(QPointF dst_pos)
{
    float width = 7;
    float height = 25;

    QPolygonF triangle;
    triangle.append(QPointF(-width, 0));
    triangle.append(QPointF(0., -height));
    triangle.append(QPointF(width, 0));
    triangle.append(QPointF(-width, 0));

    arrow->setPolygon(triangle);

    QPointF src = static_cast<node *>(this_node)->get_center_position();
    float rotation = node_link::get_rotation(src, dst_pos);

    arrow->setRotation(rotation);
    arrow->setPos(dst_pos);
}

void output_link::link_refresh(QPointF dst_pos)
{
    QPointF src = static_cast<node *>(this_node)->get_center_position();
    QLineF line = {src, dst_pos};
    link->setLine(line);
}

void output_link::refresh()
{
    node *_this_node = static_cast<node *>(this_node);

    if (_this_node->output_is_connected())
    {
        set_visible(false);
        return;
    }

    int link_size = 0;
    if (_this_node->get_type() == "dot")
        link_size = 10;
    int height_node = _this_node->get_size().height() / 2;
    QPointF src_pos = _this_node->get_center_position();
    QPointF dst_pos = {src_pos.x(), src_pos.y() + height_node + link_size};

    refresh_arrow(dst_pos);
    link_refresh(dst_pos);
    set_visible(true);

    // bounding box
    int width = 40;
    int height = 50;
    QRect rect(-width / 2, 0, width, height);

    this->setRect(rect);
    this->setPos(src_pos.x(), dst_pos.y());
    //
}

void output_link::set_visible(bool visible)
{
    this->setVisible(visible);
    arrow->setVisible(visible);
    link->setVisible(visible);
}

void output_link::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // tiene que existir el 'mousePressEvent' para que funcione el
    // 'mouseMoveEvent'

    QPointF pos = mapToScene(event->pos());
    link_refresh(pos);

    node *_this_node = static_cast<node *>(this_node);
    static_cast<node_view *>(_node_view)->set_output_link_node(_this_node);
}

void output_link::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    refresh();
}

void output_link::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
    link_refresh(pos);
    refresh_arrow(pos);
}
