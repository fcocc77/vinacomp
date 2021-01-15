#ifndef NODE_GRAPH_H
#define NODE_GRAPH_H

#include <QWidget>
#include <QJsonObject>
#include <QVBoxLayout>

#include <properties.h>
#include <maker.h>
#include <nodes_load.h>
#include <node_view.h>
#include <nodes_bar.h>

class node_graph : public QWidget
{
private:
    QVBoxLayout *layout;
    node_view *_node_view;
    nodes_bar *_nodes_bar;
    nodes_load *nodes_loaded;
    maker *_maker;

public:
    node_graph(
		QWidget *_vinacomp,
		QJsonObject *_project,
		properties *_properties);
    ~node_graph();

    QJsonObject get_tree() const;
    void restore_tree(QJsonObject nodes);
    QJsonObject get_scene_data() const;
    void restore_scene_data(QJsonObject scene_data);
    node_view *get_node_view() const;
    maker *get_maker() const;
};

#endif // #ifndef NODE_GRAPH_H
