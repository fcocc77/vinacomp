#ifndef NODE_GRAPH_H
#define NODE_GRAPH_H

#include <QJsonObject>
#include <QVBoxLayout>
#include <QWidget>

// Gui
#include <maker.h>
#include <node_view.h>
#include <nodes_bar.h>
#include <nodes_load.h>
#include <properties.h>
#include <trim_panel.h>

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

public:
    node_graph(QWidget *_vinacomp, project_struct *_project, properties *_properties);
    ~node_graph();

    void update_node_position_from_project();
    void restore_tree();
    QJsonObject get_scene_data() const;
    void restore_scene_data(QJsonObject scene_data);
    node_view *get_node_view() const;
    maker *get_maker() const;
};

#endif // #ifndef NODE_GRAPH_H
