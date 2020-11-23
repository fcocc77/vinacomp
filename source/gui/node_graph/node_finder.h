#ifndef NODE_FINDER_H
#define NODE_FINDER_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QJsonObject>

#include <util.h>
#include <qt.h>
#include <nodes_load.h>

class node_finder : public QWidget
{
private:
    QLineEdit *search_field;
    nodes_load *nodes;
    QWidget *_node_graph;

    void update_tree();

public:
    node_finder(QWidget *_node_graph, nodes_load *_nodes);
    ~node_finder();

    QTreeWidget *tree;

    void set_focus();
    void clear();
    void show_finder();
};

#endif //NODE_FINDER_H