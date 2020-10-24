#ifndef NODE_GRAPH_H
#define NODE_GRAPH_H

// QT5
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>
#include <QHBoxLayout>
#include <QScrollBar>

// VINA COMP
#include <node.hpp>
#include <util.hpp>

class node_graph : public QGraphicsView
{
private:
    /* data */
public:
    node_graph(/* args */);
    ~node_graph();

    QGraphicsScene *scene;

    bool panning;
    int panning_start_x, panning_start_y;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // NODE_GRAPH_H