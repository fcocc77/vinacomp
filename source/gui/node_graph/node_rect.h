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

    QGraphicsPathItem *attribute_box;
    QGraphicsPathItem *anim_item, *clone_item, *expression_item, *link_item;
    QGraphicsTextItem *anim_text, *clone_text, *expression_text, *link_text;
    bool attributes;

    bool animated, cloned, expression, with_link;

    QGraphicsPixmapItem *icon_item;

    void refresh();
    void refresh_attribute_items();
    void refresh_attribute_box();
    void set_size(int width, int height);
    void update_text(QString name, QString tips);
    void set_selected(bool selected) override;
    void set_position(float x, float y) override;
    QGraphicsPathItem *add_attribute_item(QString letter,
                                          QGraphicsTextItem *text_item);

public:
    node_rect(node_props _props, QMap<QString, node *> *_selected_nodes,
              QWidget *_node_view);
    ~node_rect();

    void set_icon(QString icon_name);
    void set_name(QString name) override;
    void set_tips(QString _tips) override;
    void set_disable(bool disable);
    inline bool is_disable() const;

    inline void set_animated(bool _animated);
    inline void set_cloned(bool _cloned);
    inline void set_expression(bool _expression);
    inline void set_link_item(bool _with_link);
};

inline void node_rect::set_animated(bool _animated)
{
    animated = _animated;
    refresh_attribute_items();
}

inline void node_rect::set_cloned(bool _cloned)
{
    cloned = _cloned;
    refresh_attribute_items();
}

inline void node_rect::set_expression(bool _expression)
{
    expression = _expression;
    refresh_attribute_items();
}

inline void node_rect::set_link_item(bool _with_link)
{
    with_link = _with_link;
    refresh_attribute_items();
}

inline bool node_rect::is_disable() const
{
    return disable;
}

#endif // NODE_RECT_H
