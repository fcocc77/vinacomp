#include "node.h"
#include <node_expression_link.h>
#include <util.h>

expression_link::expression_link(QGraphicsScene *scene, QWidget *_node_view,
                                 QGraphicsItem *_this_node)
    : this_node(_this_node)
    , arrow(new QGraphicsPolygonItem)
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

expression_link::~expression_link()
{
    delete arrow;
}

void expression_link::set_visible(bool visible)
{
    this->setVisible(visible);
    arrow->setVisible(visible);
}

void expression_link::refresh()
{
    node *_this_node = static_cast<node *>(this_node); node *linked_node = _this_node->get_linked_node();

    if (!linked_node)
    {
        setVisible(false);
        return;
    }

    QPointF src_pos = _this_node->get_center_position();
    QPointF dst_pos = linked_node->get_center_position();

    refresh_arrow(src_pos, dst_pos);

    setLine({src_pos, dst_pos});
    setVisible(true);
}

void expression_link::refresh_arrow(QPointF src, QPointF dst)
{
    float width = 5;
    float height = 20;

    QPolygonF triangle;
    triangle.append(QPointF(-width, 0));
    triangle.append(QPointF(0., height));
    triangle.append(QPointF(width, 0));
    triangle.append(QPointF(-width, 0));

    arrow->setPolygon(triangle);

    float rotation = node_link::get_rotation(src, dst);

    arrow->setRotation(rotation);

    QPointF point_in_the_middle = {(src.x() + dst.x()) / 2,
                                   (src.y() + dst.y()) / 2};

    arrow->setPos(point_in_the_middle);
}
