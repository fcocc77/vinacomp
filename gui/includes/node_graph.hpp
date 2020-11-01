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
#include <QLineEdit>
#include <QPoint>

// VINA COMP
#include <graphics_view.hpp>
#include <node.hpp>
#include <node_link.hpp>
#include <util.hpp>
#include <qt.hpp>

class node_graph : public graphics_view
{
    Q_OBJECT
private:
    QGraphicsScene *scene;

    QMap<QString, node *> *nodes;
    QMap<QString, node *> *selected_nodes;
    QMap<QString, QList<node_link *> *> *nodes_links;

    QJsonObject *link_connecting;
    bool selecting = false;
    QPoint click_position;

    QJsonObject *project;
    int *current_z_value;

    QLineEdit *node_rename_edit;

    void select_all(bool select);
    void refresh_selected_nodes();
    void setup_shortcut();
    node *get_node_from_position(QPoint position);
    void connect_node(QPoint position_node);
    void select_nodes_by_area(QPointF selection_end_point);

    // Events
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

public:
    node_graph(QJsonObject *_project);
    ~node_graph();

    node *create_node(
        QString name,
        QString icon_name,
        float x, float y,
        QString tips = "");
    void change_node_name_dialog();
    void change_node_name();
    void select_node(QString name, bool select);
    node *get_node(QString name);
    node_link *get_node_link(node *_node, int link_index);
    QJsonObject get_tree();
    void restore_tree(QJsonObject nodes);
};

#endif // NODE_GRAPH_H