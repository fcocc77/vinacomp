#ifndef NODE_LINK_H
#define NODE_LINK_H

#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPen>

class node_link : public QGraphicsLineItem
{
private:
    QGraphicsPolygonItem *arrow;

    bool disable;
    bool visible;

    void refresh_arrow(QPointF src, QPointF dst);

public:
    node_link(QGraphicsScene *scene);
    ~node_link();

    void set_visible(bool visible);
    void set_disable(bool disable);

    void set_line(QPointF src, QPointF dst);
};

#endif // NODE_LINK_H
