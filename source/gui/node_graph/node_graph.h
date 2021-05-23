#ifndef NODE_GRAPH_H
#define NODE_GRAPH_H

#include <QJsonObject>
#include <QMenu>
#include <QVBoxLayout>
#include <QWidget>

// Gui
#include <action.h>
#include <maker.h>
#include <node_view.h>
#include <node_group.h>
#include <nodes_bar.h>
#include <nodes_load.h>
#include <properties.h>

// Engine
#include <project_struct.h>

class node_graph : public QWidget
{
private:
    QVBoxLayout *layout;
    node_view *_node_view;
    nodes_bar *_nodes_bar;
    nodes_load *nodes_loaded;
    maker *_maker;
    project_struct *project;
    node_group *_node_group;
    QString group_name;
    QWidget *_vinacomp;

    QMenu *general_menu;
    QMenu *node_menu;
    void init_menu();

public:
    node_graph(QWidget *_vinacomp, project_struct *_project,
               properties *_properties, QString group_name = "",
               node_group *_node_group = nullptr);
    ~node_graph();

    // acciones publicas
    action *show_expressions_links;
    action *show_grid_action;


    void save_nodes_attributes_to_project();
    void restore_tree();
    void clear_tree();
    void save_scene_data();
    void restore_scene_data();
    inline node_view *get_node_view() const;
    inline maker *get_maker() const;
    inline bool is_group() const;
    inline QString get_group_name() const;
    inline void set_group_name(QString group_name);
    inline node_group *get_node_group() const;
    inline nodes_bar *get_nodes_bar() const;
    inline nodes_load *get_nodes_loaded() const;
};

inline nodes_load *node_graph::get_nodes_loaded() const
{
    return nodes_loaded;
}

inline nodes_bar *node_graph::get_nodes_bar() const
{
    return _nodes_bar;
}

inline node_group *node_graph::get_node_group() const
{
    return _node_group;
}

inline void node_graph::set_group_name(QString _group_name)
{
    group_name = _group_name;
}

inline QString node_graph::get_group_name() const
{
    return group_name;
}

inline bool node_graph::is_group() const
{
    return !group_name.isEmpty();
}

inline node_view *node_graph::get_node_view() const
{
    return _node_view;
}

inline maker *node_graph::get_maker() const
{
    return _maker;
}


#endif // #ifndef NODE_GRAPH_H
