#ifndef NODE_VIEW_H
#define NODE_VIEW_H

// QT5
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QLineEdit>
#include <QPainter>
#include <QPoint>
#include <QScrollBar>
#include <QShortcut>
#include <QWidget>

// Gui
#include "node.h"
#include <graphics_view.h>
#include <nodes_load.h>
#include <properties.h>

class node_view : public graphics_view
{
    Q_OBJECT
private:
    QGraphicsScene *scene;
    properties *_properties;
    QWidget *_vinacomp;
    nodes_load *nodes_loaded;
    QWidget *_node_graph;

    QMap<QString, node *> *nodes;
    QMap<QString, node *> *selected_nodes;

    QJsonObject *link_connecting;
    bool selecting = false;
    QPoint click_position;

    int *current_z_value;

    QLineEdit *node_rename_edit;

    void select_all(bool select);
    void setup_shortcut();
    node *get_node_from_position(QPoint position);
    void connect_node(QPoint position_node);
    void select_nodes_by_area(QPointF selection_end_point);
    void align_selected_nodes();
    void fit_view_to_nodes();
    void connect_to_viewer();
    node *get_main_viewer() const;
    node *get_selected_node() const;

    // Events
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

public:
    node_view(QWidget *_vinacomp, properties *_properties,
              nodes_load *nodes_loaded, QWidget *_node_graph);
    ~node_view();

    node *create_node(QString name, QColor color, QString type,
                      QPointF position = QPointF(), QString tips = "");

    void rename_node(node *_node, QString name, QString new_name);
    void change_node_name_dialog();
    void change_node_name();
    void select_node(QString name, bool select);
    node *get_node(QString name);
    inline QMap<QString, node *> *get_nodes();
    node_link *get_node_link(node *_node, int link_index);
    void set_visible_ghost_dots(bool visible);
signals:
    void right_click();
    void clicked();
};

inline QMap<QString, node *> *node_view::get_nodes()
{
    return nodes;
}

#endif // NODE_GRAPH_VIEW_H
