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
    QGraphicsRectItem *corner;
    QSize corner_size;

    int title_area_height;
    bool clicked_body_area;
    bool resizing;

    QList<node *> nodes_to_drag;
    QList<QPointF> nodes_click_position;
    QSize corner_click_diff;

    void change_size_rectangle(int _width, int _height);
    QColor get_random_color() const;
    void resize(QSize size);
    void refresh_corner();

    void set_selected(bool enable) override;
    void set_position(float x, float y) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
    node_backdrop(node_props _props, QMap<QString, node *> *_selected_nodes,
                  QWidget *node_view);
    ~node_backdrop();

    void set_tips(QString _tips);
    bool is_under_selector(QRectF selector);
    inline bool is_clicked_title_area() const;
};

inline bool node_backdrop::is_clicked_title_area() const
{
    return !clicked_body_area;
}

#endif // NODE_BACKDROP_H
