#ifndef NODE_GROUP_H
#define NODE_GROUP_H

#include "node_rect.h"

class node_group : public node_rect
{
private:
    QString icon_name;
    QGraphicsTextItem *tips_text;

    node_props props;

    QWidget *group_node_graph;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

public:
    node_group(node_props _props, QMap<QString, node *> *_selected_nodes,
               QWidget *_node_graph);
    ~node_group();

    inline QWidget *get_node_graph() const;
    void open_group(bool add_to_panel = true);
};

inline QWidget *node_group::get_node_graph() const
{
    return group_node_graph;
}

#endif // NODE_GROUP_H
