#ifndef NODE_LINK_GHOST_DOT_H
#define NODE_LINK_GHOST_DOT_H

#include <QGraphicsEllipseItem>
#include <QWidget>

class ghost_dot : public QGraphicsEllipseItem
{
private:
    QWidget *_node_graph;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
    ghost_dot(int size, QWidget *_node_graph);
    ~ghost_dot();
};

#endif // NODE_LINK_GHOST_DOT_H
