#ifndef NODE_LINKS_H
#define NODE_LINKS_H

#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPen>

#include <node_link.h>

class node_links : public QGraphicsLineItem
{
private:
    QList<node_link *> links;
    QGraphicsItem *this_node;
    QGraphicsScene *scene;

    void make_links(int count);

public:
    node_links(QGraphicsScene *scene, QWidget *_node_view,
               QGraphicsItem *_this_node);
    ~node_links();

    void refresh();
    void set_visible(bool visible);
    void set_disable(bool disable);
};

#endif // NODE_LINKS_H
