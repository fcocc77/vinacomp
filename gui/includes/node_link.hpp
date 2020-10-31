#ifndef NODE_LINK_H
#define NODE_LINK_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QPoint>
#include <QString>

#include <math.h>

#include <util.hpp>
#include <node.hpp>

class node_link : public QGraphicsLineItem
{
private:
    QGraphicsScene *scene;
    node *_node;
    node *connected_node;
    QJsonObject *link_connecting;
    int link_size;
    int index;

    QGraphicsPolygonItem *arrow;

    float arrow_refresh(QPointF point_a, QPointF point_b);
    void link_refresh(QPointF point_a, QPointF point_b);
    QLineF subtract_distance_line(QLineF line, float distance);

    // Event
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

public:
    node_link(int _index,
              QGraphicsScene *_scene,
              node *_node,
              QJsonObject *_link_connecting);
    ~node_link();

    void refresh();
    void update_connection();
    void connect_node(node *_node);
    void disconnect_node();
    void set_selected(bool enable);
};

#endif // NODE_LINK_H