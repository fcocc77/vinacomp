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
#include <QShortcut>
#include <QJsonObject>

// VINA COMP
#include <graphics_view.hpp>
#include <node.hpp>
#include <util.hpp>
#include <qt.hpp>

class node_graph : public graphics_view
{
    Q_OBJECT
private:
    void select_all(bool select);
    void refresh_selected_nodes();
    void setup_shortcut();
    QGraphicsScene *scene;

    QList<node *> *nodes;
    QList<node *> *selected_nodes;
    QJsonObject *project;

    // Events
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    node_graph(QJsonObject *_project);
    ~node_graph();

    node *add_node(QString name, int x, int y);
};

#endif // NODE_GRAPH_H