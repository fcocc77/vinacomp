#ifndef NODE_EXPRESSION_LINK_H
#define NODE_EXPRESSION_LINK_H

#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPen>

class expression_link : public QGraphicsLineItem
{
private:
    QGraphicsItem *this_node;

public:
    expression_link(QGraphicsScene *scene, QWidget *_node_view,
                    QGraphicsItem *_this_node);
    ~expression_link();

    void refresh();
};

#endif // NODE_EXPRESSION_LINK_H
