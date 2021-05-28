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
#include <node_output_link.h>
#include <node_expression_link.h>
#include <nodes_load.h>
#include <properties.h>
#include <trim_panel.h>

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
    QSize size;
    int z_value;
    properties *_properties;
    QWidget *vinacomp;
    nodes_load *nodes_loaded;
    project_struct *project;
    bool from_project;
};

class node : public QGraphicsPathItem
{
private:
    trim_panel *_trim_panel;
    QWidget *_node_view;

    QMap<QString, node *> *nodes_connected_to_the_inputs;
    QMap<QString, node *> *nodes_connected_to_the_output;
    QMap<QString, node *> *selected_nodes;
    QList<node_link *> *links;
    QList<bool> connected_indexs;

    QMap<QString, QPointF> selected_nodes_start_position;

    bool selected, is_backdrop;

    QPointF _freeze_position;
    output_link *_output_link;

    expression_link *_expression_link;
    node *linked_node;
    QList<node *> linked_nodes;

    QPointF *center_position;

    node_link *get_close_link() const;
    void insert_in_between();
    void show_close_link();
    void snap_to_node(node *_node, QPointF this_node_pos, float &x_snap,
                      float &y_snap);
    void add_linked_node(node *_node);
    void remove_linked_node(node *_node);

protected:
    nodes_load *nodes_loaded;
    QPointF click_position;
    node_props props;

    int minimum_width;
    int minimum_height;
    int current_width;
    int current_height;

    QString name;
    QString label;
    QString group_name;
    QString tips;
    QString type;
    QString icon_name;
    QColor color;

    inline void set_size(int width, int height);
    inline void set_minimum_size(int minimum_width, int minimum_height);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public:
    node(node_props _props, QMap<QString, node *> *_selected_nodes,
         QWidget *_node_graph = nullptr);
    ~node();

    void unlink_all();
    inline QJsonObject *get_params() const;
    virtual void set_name(QString name);
    void rename(QString name);
    void set_group_name(QString group_name);
    void set_linked(node *linked_node);
    void set_linked(QString node_name);
    void set_icon_name(QString name);
    virtual inline void set_tips(QString tips);
    inline QString get_name() const;
    inline QString get_label() const;
    virtual void set_position(float x, float y);
    virtual void set_selected(bool enable);
    QMap<QString, node *> *get_output_nodes() const;
    inline void add_output_node(node *_node);
    inline void remove_output_node(node *_node);
    inline void add_input_node(node *_node);
    inline void remove_input_node(node *_node);
    inline bool is_selected() const;
    inline QColor get_color() const;
    virtual inline void set_color(QColor color);
    void refresh();
    inline QList<node_link *> *get_links() const;
    node_link *get_link(int index = -1) const;
    inline trim_panel *get_trim_panel() const;
    QString get_type() const;
    inline QSize get_size() const;
    void make_panel(bool float_panel = false);
    QPointF get_center_position() const;
    void set_center_position(float x, float y);
    inline void freeze_position();
    inline QPointF get_freeze_position() const;
    inline void set_connected_index(int index, bool connected);
    inline QList<bool> get_connected_indexs();
    inline QMap<QString, node *> *get_input_nodes() const;
    inline bool output_is_connected() const;
    QList<node_link*> get_output_links() const;
    inline QString get_tips() const;
    inline node *get_linked_node() const;
    inline QString get_linked_node_name() const;
    inline expression_link *get_expression_link() const;
    void set_visible_expression_link(bool visible);
    inline QString get_group_name() const;
    inline QWidget *get_node_view() const;
    inline output_link *get_output_link() const;
};

inline output_link *node::get_output_link() const
{
    return _output_link;
}

inline QWidget *node::get_node_view() const
{
    return _node_view;
}

inline QString node::get_group_name() const
{
    return group_name;
}

inline expression_link *node::get_expression_link() const
{
    return _expression_link;
}

inline QString node::get_linked_node_name() const
{
    if (linked_node)
        return linked_node->get_name();

    return "";
}

inline node *node::get_linked_node() const
{
    return linked_node;
}

inline QJsonObject *node::get_params() const
{
    return props.project->nodes.value(get_name()).params;
}

inline QString node::get_tips() const
{
    return tips;
}

inline void node::set_size(int width, int height)
{
    current_width = width;
    current_height = height;
}

inline void node::set_minimum_size(int _minimum_width, int _minimum_height)
{
    minimum_width = _minimum_width;
    minimum_height = _minimum_height;
}

inline void node::set_color(QColor _color)
{
    color = _color;
}

inline bool node::output_is_connected() const
{
    return !nodes_connected_to_the_output->keys().empty();
}

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

inline QString node::get_label() const
{
    // si el nombre tiene punto esta dentro de un grupo y el 'label' es solo la
    // ultima parte, sin el nombre del grupo
    return name.split('.').last();
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

inline QMap<QString, node *> *node::get_input_nodes() const
{
    return nodes_connected_to_the_inputs;
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
