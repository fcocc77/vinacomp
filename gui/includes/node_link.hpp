#ifndef NODE_LINK_H
#define NODE_LINK_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPoint>

#include <util.hpp>
#include <node.hpp>

class node_link : public QGraphicsLineItem
{
private:
    QGraphicsScene *scene;
    node *_node;

    // Event
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

public:
    node_link(QGraphicsScene *_scene, node *_node);
    ~node_link();

    void refresh();
};

#endif // NODE_LINK_H