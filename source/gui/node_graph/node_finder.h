#ifndef NODE_FINDER_H
#define NODE_FINDER_H

#include <QJsonObject>
#include <QLineEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include <QKeyEvent>

#include <nodes_load.h>

class node_finder : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *search_field;
    nodes_load *nodes;
    QWidget *_node_view;
    QString last_node_created;

    void update_tree();
    void create_node();
    void select_first_item();
    QTreeWidgetItem *get_item(QString key) const;

    void keyPressEvent(QKeyEvent *event) override;

public:
    node_finder(QWidget *_node_view, nodes_load *_nodes);
    ~node_finder();

    QTreeWidget *tree;

    inline void set_focus();
    void show_finder();
signals:
    void created(QString node);
};

inline void node_finder::set_focus()
{
    search_field->setFocus();
}

#endif // NODE_FINDER_H
