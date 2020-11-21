#ifndef NODE_FINDER_H
#define NODE_FINDER_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <node_graph.h>
#include <QJsonObject>

#include <util.h>
#include <qt.h>

class node_finder : public QWidget
{
private:
    QLineEdit *search_field;
    QJsonObject *effects;
    node_graph *_node_graph;

    void update_tree();

public:
    node_finder(node_graph *_node_graph, QJsonObject *_effects);
    ~node_finder();

    QTreeWidget *tree;

    void set_focus();
    void clear();
    void show_finder();
};

#endif //NODE_FINDER_H