#ifndef NODE_FINDER_H
#define NODE_FINDER_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <node_graph.hpp>

#include <util.hpp>
#include <qt.hpp>

class node_finder : public QWidget
{
private:
    QLineEdit *search_field;
    QTreeWidget *tree;

    node_graph *_node_graph;

    void update_tree();

public:
    node_finder(node_graph *_node_graph);
    ~node_finder();

    void set_focus();
    void clear();
    void show_finder();
};

#endif //NODE_FINDER_H