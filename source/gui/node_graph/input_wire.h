#ifndef NODE_WIRE_H
#define NODE_WIRE_H

#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QPoint>
#include <QString>

#include <input_wire_ghost_dot.h>

// Engine
#include <project_struct.h>

class input_wire : public QGraphicsRectItem
{
private:
    QGraphicsScene *scene;
    QGraphicsItem *this_node;
    QGraphicsItem *connected_node;
    ghost_dot *_ghost_dot;
    QJsonObject *input_connecting;
    project_struct *project;
    QWidget *_vinacomp;

    int wire_size;
    int index;
    bool dragging;
    bool visible;
    QString label;
    bool has_mask;
    bool ghost_dot_visible;
    int ghost_dot_size;

    QGraphicsLineItem *wire, *ghost_wire_a, *ghost_wire_b;
    QGraphicsPolygonItem *arrow;
    QGraphicsTextItem *text;

    float arrow_refresh(QPointF point_a, QPointF point_b);
    void wire_refresh(QPointF point_a, QPointF point_b);
    void bbox_refresh(QPointF point_a, QPointF point_b);
    void text_refresh(QPointF point_a, QPointF point_b);

    QLineF subtract_distance_line(QLineF line, float distance);
    float get_long(QPointF point_a, QPointF point_b);
    QPointF get_center(QPointF point_a, QPointF point_b) const;
    void update_visibility();
    QLineF get_line_from_node() const;
    bool safe_connection(QGraphicsItem *node_to_connect) const;

    inline void set_visible(bool visible);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
    input_wire(QString input_label, bool has_mask, int _index,
              QGraphicsScene *_scene, QGraphicsItem *_node,
              QJsonObject *_input_connecting, project_struct *_project,
              QWidget *_vinacomp, QWidget *_node_graph);
    ~input_wire();

    static float get_rotation(QPointF point_a, QPointF point_b);

    void refresh();
    void update_connection();
    bool connect_node(QGraphicsItem *_node, bool prevent_loop = true);
    void disconnect_node();
    void set_selected(bool enable);
    void set_ghost_wire(bool visible, QPointF break_point = {});
    inline QGraphicsItem *get_connected_node() const;
    inline QGraphicsItem *get_this_node() const;
    inline void set_visible_ghost_dot(bool visible);
    inline bool is_connected() const;
    inline QPointF get_center_position() const;
    void insert_node_in_between(QGraphicsItem *_node);
};

inline QGraphicsItem *input_wire::get_this_node() const
{
    return this_node;
}

inline QPointF input_wire::get_center_position() const
{
    QLineF line = get_line_from_node();
    return get_center(line.p1(), line.p2());
}

inline bool input_wire::is_connected() const
{
    return connected_node;
}

inline void input_wire::set_visible_ghost_dot(bool visible)
{
    ghost_dot_visible = visible;
    refresh();
}

inline QGraphicsItem *input_wire::get_connected_node() const
{
    return connected_node;
}

inline void input_wire::set_visible(bool _visible)
{
    visible = _visible;
    this->setVisible(visible);
    wire->setVisible(visible);
    arrow->setVisible(visible);
}

#endif // NODE_WIRE_H
