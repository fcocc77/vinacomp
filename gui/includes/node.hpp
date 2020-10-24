#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QPen>

#include <util.hpp>

class node : public QGraphicsRectItem
{
private:
    QList<QGraphicsLineItem *> *inputs;
    QGraphicsScene *scene;
    QList<node *> *nodes;

    int width;
    int height;
    QLabel *label;
    bool selected;

public:
    node(QGraphicsScene *_scene, QList<node *> *_nodes);
    ~node();

    void inputs_refresh();
    QGraphicsLineItem get_input(int index);
    void add_input();
    void connect_input(int index, node *_node);
    void set_name(QString name);
    void set_position(int x, int y);
    void select(bool _selected);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // NODE_H