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
#include <QPainter>

// VINA COMP
#include <node.hpp>
#include <util.hpp>

class node_graph : public QGraphicsView
{
    Q_OBJECT
private:
    QGraphicsScene *scene;

    bool panning;
    bool pressed;
    int panning_start_x, panning_start_y;
    QList<node *> *nodes;

    // Events
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public:
    node_graph(/* args */);
    ~node_graph();

    void add_node(QString name, int x, int y);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // NODE_GRAPH_H