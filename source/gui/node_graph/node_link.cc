#include "node.h"
#include <node_link.h>
#include <util.h>

node_link::node_link(QGraphicsScene *scene)
    : arrow(new QGraphicsPolygonItem)
    , disable(false)
    , visible(false)
{
    QPen pen(Qt::green);
    pen.setWidth(1);
    this->setPen(pen);
    scene->addItem(this);

    // Flecha
    QBrush arrow_brush(Qt::green);
    arrow->setBrush(arrow_brush);
    arrow->setPen(QPen(Qt::green, 0));
    scene->addItem(arrow);
}

node_link::~node_link()
{
    delete arrow;
}

void node_link::set_line(QPointF src, QPointF dst)
{
    refresh_arrow(src, dst);
    setLine({src, dst});
}

void node_link::set_visible(bool _visible)
{
    if (disable)
        _visible = false;

    visible = _visible;
    this->setVisible(visible);
    arrow->setVisible(visible);
}

void node_link::set_disable(bool _disable)
{
    disable = _disable;

    if (disable)
        set_visible(false);
}

void node_link::refresh_arrow(QPointF src, QPointF dst)
{
    float width = 5;
    float height = 20;

    QPolygonF triangle;
    triangle.append(QPointF(-width, 0));
    triangle.append(QPointF(0., height));
    triangle.append(QPointF(width, 0));
    triangle.append(QPointF(-width, 0));

    arrow->setPolygon(triangle);

    float rotation = input_wire::get_rotation(src, dst);

    arrow->setRotation(rotation);

    QPointF point_in_the_middle = {(src.x() + dst.x()) / 2,
                                   (src.y() + dst.y()) / 2};

    arrow->setPos(point_in_the_middle);
}
