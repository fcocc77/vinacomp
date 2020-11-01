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

#include <util.hpp>

class node : public QGraphicsPathItem
{
private:
    QGraphicsScene *scene;
    QMap<QString, node *> *nodes_connected_to_the_inputs;
    QMap<QString, node *> *nodes_connected_to_the_output;
    QMap<QString, node *> *selected_nodes;

    QPointF *center_position;
    bool selected = false;

    QPointF click_position_on_node;

    int minimum_width;
    int minimum_height;
    int current_width;
    int current_height;

    int *current_z_value;

    int icon_area_width;
    QGraphicsTextItem *name;
    QGraphicsTextItem *tips;

    void change_size_rectangle(int _width, int _height);

    // eventos
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

public:
    node(QGraphicsScene *_scene,
         int *_current_z_value,
         QMap<QString, node *> *_selected_nodes);
    ~node();

    void set_name(QString name);
    void set_tips(QString tips);
    QString get_name();
    void set_position(float x, float y);
    QPointF get_center_position();
    QList<int> get_size();
    void set_icon(QString icon_name);
    void set_selected(bool enable);
    QMap<QString, node *> *get_output_nodes();
    void add_output_node(node *_node);
    void remove_output_node(node *_node);
    void add_input_node(node *_node);
    void remove_input_node(node *_node);
    bool is_selected();
};

#endif // NODE_H