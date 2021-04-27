#ifndef NODE_RECT_H
#define NODE_RECT_H

#include "node.h"

class node_rect : public node
{
private:
    QString icon_name;

    const int icon_area_width;
    bool disable;

    QGraphicsTextItem *name_text;
    QGraphicsTextItem *tips_text;
    
    QGraphicsLineItem *disable_line_a;
    QGraphicsLineItem *disable_line_b;

    void refresh();
    void set_size(int width, int height);
    void update_text(QString name, QString tips);
    void set_selected(bool selected) override;

public:
    node_rect(node_props _props, QMap<QString, node *> *_selected_nodes,
              QWidget *_node_view);
    ~node_rect();

    void set_icon(QString icon_name);
    void set_name(QString name) override;
    void set_tips(QString _tips) override;
    void set_disable(bool disable);
    inline bool is_disable() const;
};

inline bool node_rect::is_disable() const
{
    return disable;
}

#endif // NODE_RECT_H
