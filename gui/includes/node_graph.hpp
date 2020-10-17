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

// VINA COMP
#include <node.hpp>

class node_graph : public QWidget
{
private:
    /* data */
public:
    node_graph(/* args */);
    ~node_graph();
};

#endif // NODE_GRAPH_H