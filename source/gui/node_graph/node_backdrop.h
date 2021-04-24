#ifndef NODE_BACKDROP_H
#define NODE_BACKDROP_H

#include "../node_graph/node.h"

class node_backdrop : public node
{
private:
    node_props props;

    QString icon_name;
    QGraphicsTextItem *tips_text;
    QWidget *_node_view;

    int title_area_height;
    bool clicked_title_area;

    QList<node *> nodes_to_drag;
    QList<QPointF> nodes_click_position;

    void change_size_rectangle(int _width, int _height);
    QColor get_random_color() const;

    void set_selected(bool enable) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
    node_backdrop(node_props _props, QMap<QString, node *> *_selected_nodes,
                  QWidget *node_view);
    ~node_backdrop();

    void set_tips(QString _tips);
};

#endif // NODE_BACKDROP_H
