#ifndef NODE_LINK_H
#define NODE_LINK_H

#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QPoint>
#include <QString>

#include <math.h>

// Gui
#include <util.h>

// Engine
#include <project_struct.h>

class node_link : public QGraphicsRectItem
{
private:
    QGraphicsScene *scene;
    QGraphicsItem *this_node;
    QGraphicsItem *connected_node;
    QJsonObject *link_connecting;
    project_struct *project;
    QWidget *_vinacomp;

    int link_size;
    int index;
    bool dragging = false;

    QGraphicsLineItem *link;
    QGraphicsPolygonItem *arrow;
    QGraphicsTextItem *text;

    float arrow_refresh( QPointF point_a, QPointF point_b );
    void link_refresh( QPointF point_a, QPointF point_b );
    void bbox_refresh( QPointF point_a, QPointF point_b );
    void text_refresh( QPointF point_a, QPointF point_b );

    QLineF subtract_distance_line( QLineF line, float distance );
    float get_rotation( QPointF point_a, QPointF point_b );
    float get_long( QPointF point_a, QPointF point_b );
    QPointF get_center( QPointF point_a, QPointF point_b );

    // Event
    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event );

public:
    node_link( int _index, QGraphicsScene *_scene, QGraphicsItem *_node,
               QJsonObject *_link_connecting, project_struct *_project, QWidget *_vinacomp );
    ~node_link();

    void refresh();
    void update_connection();
    void connect_node( QGraphicsItem *_node );
    void disconnect_node();
    void set_selected( bool enable );
    QGraphicsItem *get_connected_node();
};

#endif // NODE_LINK_H
