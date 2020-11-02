#ifndef NODE_FINDER_H
#define NODE_FINDER_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <util.hpp>

class node_finder : public QWidget
{
private:
    QLineEdit *search_field;
    QTreeWidget *tree;

    void update_tree();

public:
    node_finder(QWidget *parent = 0);
    ~node_finder();

    void set_focus();
    void clear();
};

#endif //NODE_FINDER_H