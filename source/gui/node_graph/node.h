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

#include <util.h>
#include <properties.h>
#include <trim_panel.h>
#include <node_link.h>

class node : public QGraphicsPathItem
{
private:
    trim_panel *panel;
    properties *_properties;

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
    QGraphicsTextItem *name;
    QGraphicsTextItem *tips;

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
         trim_panel *_panel,
         properties *_properties);
    ~node();

    void set_name(QString name);
    void set_tips(QString tips);
    QString get_name();
    void set_position(float x, float y);
    QPointF get_center_position();
    QSize get_size();
    void set_icon(QString icon_name);
    void set_selected(bool enable);
    QMap<QString, node *> *get_output_nodes();
    void add_output_node(node *_node);
    void remove_output_node(node *_node);
    void add_input_node(node *_node);
    void remove_input_node(node *_node);
    bool is_selected();
    QColor get_color();
    void refresh();
    QList<node_link *> *get_links();
};

#endif // NODE_H