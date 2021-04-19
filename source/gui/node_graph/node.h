#ifndef NODE_H
#define NODE_H

#include <QFocusEvent>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

// Gui
#include <node_link.h>
#include <nodes_load.h>
#include <properties.h>
#include <trim_panel.h>
#include <viewer.h>

// Engine
#include <project_struct.h>

struct node_props
{
    QGraphicsScene *scene;
    int *current_z_value;
    QJsonObject *link_connecting;
    QColor color;
    QString type;
    QString name;
    QString tips;
    properties *_properties;
    QWidget *vinacomp;
    nodes_load *nodes_loaded;
    project_struct *project;
};

class node : public QGraphicsPathItem
{
private:
    trim_panel *_trim_panel;
    viewer *_viewer;

    node_props props;

    QMap<QString, node *> *nodes_connected_to_the_inputs;
    QMap<QString, node *> *nodes_connected_to_the_output;
    QMap<QString, node *> *selected_nodes;
    QList<node_link *> *links;
    QList<bool> connected_indexs;

    QMap<QString, QPointF> selected_nodes_start_position;

    bool selected = false;
    QPointF _freeze_position;

    QPointF *center_position;

protected:
    nodes_load *nodes_loaded;
    QPointF click_position;

    int minimum_width;
    int minimum_height;
    int current_width;
    int current_height;

    QString name;
    QString tips;
    QString type;
    QString icon_name;
    QColor color;

    void set_size(int minimum_width, int minimum_height);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

public:
    node(node_props _props, QMap<QString, node *> *_selected_nodes);
    ~node();

    void set_name(QString name);
    void set_icon_name(QString name);
    inline void set_tips(QString tips);
    inline QString get_name() const;
    void set_position(float x, float y);
    void set_position(QPointF position);
    void set_selected(bool enable);
    QMap<QString, node *> *get_output_nodes() const;
    inline void add_output_node(node *_node);
    inline void remove_output_node(node *_node);
    inline void add_input_node(node *_node);
    inline void remove_input_node(node *_node);
    inline bool is_selected() const;
    inline QColor get_color() const;
    void refresh();
    inline QList<node_link *> *get_links() const;
    inline trim_panel *get_trim_panel() const;
    QString get_type() const;
    inline QSize get_size() const;
    void make_panel();
    QPointF get_center_position() const;
    inline void freeze_position();
    inline QPointF get_freeze_position() const;
    inline void set_connected_index(int index, bool connected);
    inline QList<bool> get_connected_indexs();
    void refresh_links();
};

inline void node::set_connected_index(int index, bool connected)
{
    connected_indexs[index] = connected;
}

inline QList<bool> node::get_connected_indexs()
{
    return connected_indexs;
}

inline void node::freeze_position()
{
    _freeze_position = this->pos();
}

inline QPointF node::get_freeze_position() const
{
    return _freeze_position;
}

inline bool node::is_selected() const
{
    return selected;
}

inline void node::set_icon_name(QString name)
{
    icon_name = name;
}

inline QSize node::get_size() const
{
    return QSize(current_width, current_height);
}

inline QString node::get_name() const
{
    return name;
}

inline void node::set_tips(QString _tips)
{
    tips = _tips;
}

inline trim_panel *node::get_trim_panel() const
{
    return _trim_panel;
}

inline QColor node::get_color() const
{
    return color;
}

inline void node::add_output_node(node *_node)
{
    nodes_connected_to_the_output->insert(_node->get_name(), _node);
}

inline void node::remove_output_node(node *_node)
{
    nodes_connected_to_the_output->remove(_node->get_name());
}

inline QMap<QString, node *> *node::get_output_nodes() const
{
    return nodes_connected_to_the_output;
}

inline void node::add_input_node(node *_node)
{
    nodes_connected_to_the_inputs->insert(_node->get_name(), _node);
}

inline void node::remove_input_node(node *_node)
{
    nodes_connected_to_the_inputs->remove(_node->get_name());
}

inline QList<node_link *> *node::get_links() const
{
    return links;
}

inline QString node::get_type() const
{
    return type;
}

#endif // NODE_H
