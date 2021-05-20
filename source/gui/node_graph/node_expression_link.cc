#include "node.h"
#include <node_expression_link.h>
#include <util.h>

expression_link::expression_link(QGraphicsScene *scene, QWidget *_node_view,
                                 QGraphicsItem *_this_node)
    : this_node(_this_node)
{
    QPen pen(Qt::green);
    pen.setWidth(1);
    this->setPen(pen);
    scene->addItem(this);
}

expression_link::~expression_link() {}

void expression_link::refresh()
{
    node *_this_node = static_cast<node *>(this_node);
    node *linked_node = _this_node->get_linked_node();

    if (!linked_node)
    {
        setVisible(false);
        return;
    }

    QPointF src_pos = _this_node->get_center_position();
    QPointF dst_pos = linked_node->get_center_position();

    setLine({src_pos, dst_pos});
    setVisible(true);
}
