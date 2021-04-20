#ifndef NODE_RECT_H
#define NODE_RECT_H

#include "node.h"

class node_rect : public node
{
private:
    QString icon_name;

    const int icon_area_width;

    QGraphicsTextItem *name_text;
    QGraphicsTextItem *tips_text;

    void change_size_rectangle(int _width, int _height);

public:
    node_rect(node_props _props, QMap<QString, node *> *_selected_nodes,
              QWidget *_node_view);
    ~node_rect();

    void set_icon(QString icon_name);
    void set_name(QString name);
    void set_tips(QString _tips);
};

#endif // NODE_RECT_H
