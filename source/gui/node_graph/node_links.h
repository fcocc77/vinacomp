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
    void disable_all_links();

public:
    node_links(QGraphicsScene *scene, QWidget *_node_view,
               QGraphicsItem *_this_node);
    ~node_links();

    void refresh();
    void set_visible(bool visible);

    void enable_links(int links_count);
};

#endif // NODE_LINKS_H
