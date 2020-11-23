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

public:
    node_graph(QJsonObject *_project,
               properties *_properties);
    ~node_graph();
};

#endif // #ifndef NODE_GRAPH_H
