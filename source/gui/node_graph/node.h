#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QPainter>
#include <QFocusEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsRectItem>

// Gui
#include <util.h>
#include <properties.h>
#include <trim_panel.h>
#include <node_link.h>
#include <viewer.h>
#include <nodes_load.h>

// Engine
#include <project_struct.h>

class node : public QGraphicsPathItem
{
private:
	QWidget *_vinacomp;
    trim_panel *_trim_panel;
    properties *_properties;
	viewer *_viewer;
	nodes_load *nodes_loaded;
	project_struct *project;

    QGraphicsScene *scene;
    QMap<QString, node *> *nodes_connected_to_the_inputs;
    QMap<QString, node *> *nodes_connected_to_the_output;
    QMap<QString, node *> *selected_nodes;
    QList<node_link *> *links;

    QMap<QString, QPointF> selected_nodes_start_position;

    QPointF *center_position;
    bool selected = false;

    QColor color;

    QPointF start_position;
    QPointF click_position;

    const int minimum_width;
    const int minimum_height;
    int current_width;
    int current_height;

    int *current_z_value;

    const int icon_area_width;
    QGraphicsTextItem *name_text;
    QGraphicsTextItem *tips_text;
	QString type;
	QString icon_name;

    void change_size_rectangle(int _width, int _height);

    // eventos
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

public:
    node(QGraphicsScene *_scene,
         int *_current_z_value,
         QJsonObject *_link_connecting,
         QMap<QString, node *> *_selected_nodes,
         int inputs,
         QColor color,
		 QString type,
		 QString name,
		 QString tips,
         properties *_properties,
		 QWidget *_vinacomp,
		 nodes_load *nodes_loaded,
		 project_struct *project
	);
    ~node();

    void set_name(QString name);
    void set_tips(QString tips);
    QString get_name() const;
    void set_position(float x, float y);
    QPointF get_center_position() const;
    QSize get_size() const;
    void set_icon(QString icon_name);
    void set_selected(bool enable);
    QMap<QString, node *> *get_output_nodes() const;
    void add_output_node(node *_node);
    void remove_output_node(node *_node);
    void add_input_node(node *_node);
    void remove_input_node(node *_node);
    bool is_selected() const;
    QColor get_color() const;
    void refresh();
    QList<node_link *> *get_links() const;
	trim_panel *get_trim_panel() const;
	QString get_type() const;
	void make_panel();
};

#endif // NODE_H
