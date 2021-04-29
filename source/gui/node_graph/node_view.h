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
    project_struct *project;

    QMap<QString, node *> *nodes;
    QMap<QString, node *> *selected_nodes;
    QMap<QString, node *> copied_nodes;

    QJsonObject *link_connecting;
    node *output_link_node;
    bool selecting = false;
    QPoint click_position;
    QGraphicsRectItem *selection_box;
    bool insertion_between_nodes;

    int *current_z_value;

    QLineEdit *node_rename_edit;

    void select_all(bool select);
    void setup_shortcut();
    void connect_node(QPoint position_node);
    void extract_node(node *_node);
    void connect_node_to_selected_nodes(node *_node);
    QPointF get_min_node_separation(node *node_a, node *node_b) const;
    void select_nodes_by_area(QPointF selection_end_point);
    void align_selected_nodes();
    void fit_view_to_nodes();
    void connect_to_viewer();
    node *get_main_viewer() const;
    node *get_selected_node() const;
    void connect_output_link(QPoint position_node);
    void select_connected_nodes(node *root_node);

    // Events
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public:
    node_view(QWidget *_vinacomp, properties *_properties,
              nodes_load *nodes_loaded, project_struct *project,
              QWidget *_node_graph);
    ~node_view();

    node *create_node(node_struct node_data, bool basic_creation = false,
                      bool from_project = false);

    node *get_node_from_position(QPoint position);
    QRectF bbox_nodes(QMap<QString, node *> *_nodes) const;
    bool rename_node(node *_node, QString new_name);
    void change_node_name_dialog();
    void change_node_name();
    void disable_selected_nodes();
    void switch_inputs_a_and_b();
    void extract_selected_nodes();
    void select_node(QString name, bool select);
    void select_node(node *node, bool select);
    void delete_selected_nodes();
    void delete_node(node *_node);
    void delete_node(QString name);
    void copy_nodes();
    void paste_nodes();
    node *get_node(QString name);
    inline QMap<QString, node *> *get_nodes();
    node_link *get_node_link(node *_node, int link_index);
    void set_visible_ghost_dots(bool visible);
    void find_nodes(QString key);
    inline void set_output_link_node(node *_node);
    inline QMap<QString, node *> *get_selected_nodes() const;
signals:
    void right_click();
    void clicked();
};

inline QMap<QString, node *> *node_view::get_selected_nodes() const
{
    return selected_nodes;
}

inline void node_view::set_output_link_node(node *_node)
{
    output_link_node = _node;
}

inline QMap<QString, node *> *node_view::get_nodes()
{
    return nodes;
}

#endif // NODE_GRAPH_VIEW_H
