#ifndef NODE_BACKDROP_H
#define NODE_BACKDROP_H

#include "../node_graph/node.h"

class node_backdrop : public node
{
private:
    node_props props;

    QString icon_name;
    QGraphicsTextItem *name_text;
    QGraphicsTextItem *tips_text;
    QWidget *_node_view;
    QGraphicsLineItem *corner;
    QSize corner_size;

    QMap<QString, node *> *selected_nodes;

    int title_area_height;
    bool clicked_body_area;
    bool resizing;
    node_backdrop *parent;

    QList<node *> nodes_to_drag;
    QList<QPointF> nodes_click_position;
    QSize corner_click_diff;

    void update_text(QString name, QString tips);
    void set_title_area_height(int height);
    QColor get_random_color() const;
    void resize(QSize size);
    void refresh_corner();
    void calculate_size();
    void increase_z_value();
    void set_z_value(int value);
    bool is_inside_backdrop(node_backdrop *backdrop);
    void set_size(int width, int height);

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
    bool is_selected_nodes() const;
    inline bool is_clicked_title_area() const;
};

inline bool node_backdrop::is_clicked_title_area() const
{
    return !clicked_body_area || resizing;
}

#endif // NODE_BACKDROP_H
