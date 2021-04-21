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
    QString group_name;

    QMenu *menu;
    void init_menu();

public:
    node_graph(QWidget *_vinacomp, project_struct *_project,
               properties *_properties, QString group_name = "");
    ~node_graph();

    void update_node_position_from_project();
    void restore_tree();
    QJsonObject get_scene_data() const;
    void restore_scene_data(QJsonObject scene_data);
    inline node_view *get_node_view() const;
    inline maker *get_maker() const;
    inline bool is_group() const;
    inline QString get_group_name() const;
};

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
