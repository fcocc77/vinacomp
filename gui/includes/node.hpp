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

#include <util.hpp>

class node : public QGraphicsPathItem
{
private:
    QList<QGraphicsLineItem *> *inputs;
    QGraphicsScene *scene;
    QList<node *> *selected_nodes;
    QList<node *> *connected_nodes;

    int minimum_width;
    int minimum_height;
    int current_width;
    int current_height;

    int icon_area_width;
    bool selected;
    QGraphicsTextItem *name;

    void input_line_connect(int index);
    void change_size_rectangle(int _width, int _height);

public:
    node(QGraphicsScene *_scene, QList<node *> *_selected_nodes);
    ~node();

    void refresh();
    QGraphicsLineItem get_input(int index);
    void add_input();
    void connect_input(int index, node *_node);
    void set_name(QString name);
    void set_tips(QString tips);
    QString get_name();
    void set_position(int x, int y);
    void select(bool _selected);
    QList<int> get_size();
    void set_icon(QString icon_name);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // NODE_H