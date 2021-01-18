#ifndef NODE_VIEW_H
#define NODE_VIEW_H

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
#include <graphics_view.h>
#include <node.h>
#include <node_link.h>
#include <util.h>
#include <qt.h>
#include <trim_panel.h>
#include <properties.h>
#include <viewer.h>
#include <nodes_load.h>

class node_view : public graphics_view
{
    Q_OBJECT
private:
    QGraphicsScene *scene;
    properties *_properties;
	QWidget *_vinacomp;
	nodes_load *nodes_loaded;

    QMap<QString, node *> *nodes;
    QMap<QString, node *> *selected_nodes;

    QJsonObject *link_connecting;
    bool selecting = false;
    QPoint click_position;

    QJsonObject *project;
    int *current_z_value;

    QLineEdit *node_rename_edit;

    void select_all(bool select);
    void setup_shortcut();
    node *get_node_from_position(QPoint position);
    void connect_node(QPoint position_node);
    void select_nodes_by_area(QPointF selection_end_point);
    void align_selected_nodes();

    // Events
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

public:
    node_view(
		QWidget *_vinacomp,
        QJsonObject *_project,
        properties *_properties,
		nodes_load *nodes_loaded
	);
    ~node_view();

    node *create_node(
        QString name,
        QColor color,
		QString type,
        QPointF position = QPointF(),
		QJsonObject data = {},
        QString tips = "");
    void rename_node(node *_node, QString name, QString new_name);
    void change_node_name_dialog();
    void change_node_name();
    void select_node(QString name, bool select);
    node *get_node(QString name);
    QMap<QString, node *> *get_nodes();
    node_link *get_node_link(node *_node, int link_index);
};

#endif // NODE_GRAPH_VIEW_H
